
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "Thread.h"
#include "List.h"
#include "server.h"

#define Log(loglvl, flag, fmt, ...)

static void setnonblocking(int sock)
{
	int opts;

	opts = fcntl(sock, F_GETFL);
	if (opts < 0) {
		perror("fcntl(sock, GETFL)");
		exit(1);
	}

	opts = opts | O_NONBLOCK;
	if (fcntl(sock, F_SETFL, opts) < 0) {
		perror("fcntl(sock, SETFL, opts)");
		exit(1);
	}

	return;
}

long long eti_get_millisec(void)
{
//#define SUPPORT_MONOTONIC_TM
#ifdef  SUPPORT_MONOTONIC_TM
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return ((long long)now.tv_sec) * 1000 + now.tv_nsec / 1000000;
#else
	struct timeval tm;
	gettimeofday(&tm, 0);
	//LOGTRACE("TM sec %ld, usec %ld\n", tm.tv_sec, tm.tv_usec);
	return ((long long)tm.tv_sec) * 1000 + tm.tv_usec / 1000;
#endif
}

int tcp_recvn(int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nread = recv(fd, ptr, nleft, 0)) < 0) {
			if (errno == EINTR)
				nread = 0;	/* and call read() again */
			else if (errno == EWOULDBLOCK || errno == EAGAIN) {
				//printf("tcp rec EAGAIN,%d\n", errno);
				return (n - nleft);
			} else {
				//printf("tcp rec errno %d\n", errno);
				return (-1);
			}

		} else if (nread == 0) {
			//printf("tcp rec zero\n");
			return (-1);
			//break;                            /* EOF */
		} else {
			//printf("tcp rec:%d\n", nread);
		}

		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);	/* return >= 0 */
}

int tcp_sendn(int fd, const char *buf, int len)
{
	int send_len;
	int sum = 0;
	int n = 0;

	while (sum < len) {
		send_len = send(fd, buf + sum, len - sum, 0);

		if (send_len < 0) {
			if (errno == EINTR) {
				//return sum;
				send_len = 0;
			} else if (errno == EWOULDBLOCK || errno == EAGAIN) {
				//n++;
				//if (n > 2)
				{
					return sum;
				}
				//continue;
			} else {
				return -1;
			}
		} else if (send_len == 0) {
			//return -1;
			return sum;
		}

		sum = sum + send_len;
	}

	return sum;
}

int et_create_context(et_context * handle)
{
	et_context_data *data = NULL;
	data = calloc(1, sizeof(et_context_data));
	data->local_imsrv_lock = Thread_create_mutex();
	SL_Creat((SList *) & data->connect_tcp_client_list, 0);
	*handle = data;
	return 0;
}

int et_set_local_server_option(et_context handle, int port, int cycle,
			       int data_size)
{
	et_context_data *context_data = handle;
	context_data->local_server_listen_port = port;
	context_data->local_server_auto_send_cycle = cycle;
	context_data->local_server_auto_send_data_size = data_size;
	return 0;
}

int et_destroy_list(et_context handle, et_list * list)
{
	int rc = -1;
	et_list p = NULL;

	if (*list == NULL) {
		Log(LOG_ERROR, 1, "now list is null please check\n");
		return -1;
	}

	p = (*list)->next;

	while (p != NULL) {
		if (p->handle != NULL) {
			free(p->handle);
			p->handle = NULL;
		}
		p = p->next;
	}

	rc = SL_Clear((SList *) list);

	return rc;
}

void et_destroy_context(et_context handle)
{
	et_context_data *context_data = handle;
	et_destroy_list(context_data, &context_data->connect_tcp_client_list);
	Thread_destroy_mutex(context_data->local_imsrv_lock);
	free(context_data);
}

char auto_send_buff[1024 * 1024 * 10] = { 0 };

int server_thread_stop_flag = 0;

void *start_svr_thread(void *handle)
{
	int i = 0;
	int ret = -1;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t socklen;
	int listenfd = -1;
	int connectfd = -1;
	int sockopt = 1;
	int udp_broad_fd = -1;
	int maxfd = -1;
	int max_tcp_client = -1;
	struct timeval time_out;
	fd_set readfds;
	fd_set client_exceptfds;
	fd_set writefds;
	et_context_data *context_data = handle;

	int cmd = 0, seq = 0;
	//unsigned char user_data[MAX_UDP_PACKET_SZ] = {0};
	char userid[66] = { 0 };
	struct sockaddr_in from_addr;
	char send_buff[256] = { 0 };
	char recv_buff[2048] = { 0 };

	int auto_send_number = 0;

	int type = -1;
	unsigned char *payload;
	int payloadlen;
	char *name, *tmp_name;
	short ret_id;
	int name_len;
	//et_context_message *mm = NULL;

	int size = 0;
	int rc = -1;
	struct tcp_socket_client *tcp_client = NULL;
	//int tcp_connect_flag = 0;
	int send_udp_flag = 0;
	et_server_data server_data;
	//int msgid = 0x123;
	et_context_failure_data lsvrfail;

	if (context_data == NULL) {
		//Log(LOG_ERROR,1,"handle is NULL");
		context_data->server_thread_flag = 0;
		return 0;
	}
	printf("port:%d\n", context_data->local_server_listen_port);
	socklen = sizeof(struct sockaddr_in);
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;	//PF_INET
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(context_data->local_server_listen_port);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);	//PF_INET
	if (listenfd < 0) {
		Log(LOG_ERROR, 1, "Create listenfd Failed!");
		context_data->server_thread_flag = 0;
		return 0;
	}

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &sockopt,
		   sizeof(sockopt));

	if (bind
	    (listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {

		printf("Server Bind Port : %d Failed!",
		       context_data->local_server_listen_port);
		//lsvrfail.code = ET_ERR_LOCAL_SERVER_PORT_INUSE;
		close(listenfd);
		listenfd = -1;
		goto localsrv_err_exit;
		//return 0;
	}

	if (listen(listenfd, LENGTH_OF_LISTEN_QUEUE)) {
		close(listenfd);
		printf("Server Listen Failed!");
		context_data->server_thread_flag = 0;
		return 0;
	} else {

		printf("Local server port:%d\n",
		       context_data->local_server_listen_port);
	}

	/*udp_broad_fd = et_socket_creat_udp_broadcast_server(context_data,UDP_BROAD_CAST_PORT);

	   if (udp_broad_fd < 0)
	   {
	   close(listenfd);
	   Log(LOG_ERROR,1,"Create listenfd Failed!");
	   context_data->server_thread_flag = 0;
	   return 0;
	   } */

	printf("local server started, waitting client connect.\n");

	//init_tcp_client();
	/*if(context_data->query_data.on_success){
	   et_context_success_data smsg;
	   (*context_data->query_data.on_success)(context_data->query_data.context, &smsg);
	   } */

	memset(auto_send_buff, 0, sizeof(auto_send_buff));
	char start_the_char = '0';
	char end_the_char = '9';
	char fill_the_chr = start_the_char;
	for (i = 0; i < sizeof(auto_send_buff); i++) {
		auto_send_buff[i] = fill_the_chr++;
		if (fill_the_chr > end_the_char)
			fill_the_chr = start_the_char;
	}
	char ultrasonic1[20]={0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x02,
		              0x00, 0x00,0x3f,0x3f,  0x3f, 0x3f, 0x3f, 0x3f,
			      0x3f, 0x3f, 0x00, 0x00};
	char ultrasonic2[20]={0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x02,
		              0x00, 0x00,0xbf,0xbf,  0x9f, 0xbf, 0xbf, 0xbf,
			      0x3f, 0x3f, 0x00, 0x00};
	memcpy(auto_send_buff, ultrasonic1,20);
	memcpy(auto_send_buff, ultrasonic2,20);
	auto_send_buff[sizeof(auto_send_buff) - 1] = 0;
	long long auto_send_lasttm = eti_get_millisec();

	while (!server_thread_stop_flag) {
		maxfd = 0;
		maxfd = maxfd > listenfd ? maxfd : listenfd;
		//maxfd = maxfd > udp_broad_fd?maxfd:udp_broad_fd;

		FD_ZERO(&writefds);
		FD_ZERO(&readfds);
		FD_SET(listenfd, &readfds);
		//FD_SET(udp_broad_fd,&readfds);

		FD_ZERO(&client_exceptfds);

		max_tcp_client = 0;
		size = SL_Size((SList) context_data->connect_tcp_client_list);
		for (i = 1; i < size; i++) {
			rc = SL_Gethandle((SList) context_data->
					  connect_tcp_client_list, i,
					  (ItemHandle *) & tcp_client);
			if (rc != 0 || tcp_client == NULL) {
				Log(LOG_ERROR, 1,
				    "connect_tcp_client_list data is null please check\n");
				continue;
				//return -1;
			}
			if (tcp_client->socket_fd > 0) {
				FD_SET(tcp_client->socket_fd, &readfds);
				FD_SET(tcp_client->socket_fd,
				       &client_exceptfds);
				if (tcp_client->need_write_size) {
					FD_SET(tcp_client->socket_fd,
					       &writefds);
				}
				if (max_tcp_client < tcp_client->socket_fd)
					max_tcp_client = tcp_client->socket_fd;
			}

		}

		maxfd = maxfd > max_tcp_client ? maxfd : max_tcp_client;

		time_out.tv_sec =
		    context_data->local_server_auto_send_cycle / 1000;
		time_out.tv_usec =
		    (context_data->local_server_auto_send_cycle % 1000) * 1000;

		if (time_out.tv_sec == 0 && time_out.tv_usec == 0)
			time_out.tv_sec = 1;

		ret =
		    select(maxfd + 1, &readfds, &writefds, &client_exceptfds,
			   &time_out);

		int retselect = ret;

		if (ret < 0 && errno != EINTR) {
			Log(LOG_ERROR, 1,
			    "now it inner socket server return error\n");
			size =
			    SL_Size((SList) context_data->
				    connect_tcp_client_list);
			for (i = 1; i < size; i++) {
				rc = SL_Gethandle((SList) context_data->
						  connect_tcp_client_list, i,
						  (ItemHandle *) & tcp_client);
				if (rc != 0 || tcp_client == NULL) {
					Log(LOG_ERROR, 1,
					    "connect_tcp_client_list data is null please check\n");
					continue;
					//return -1;
				}
				if (tcp_client->socket_fd > 0) {
					Log(LOG_ERROR, 1, "close socket\n");
					close(tcp_client->socket_fd);
					// (*(context_data->callback.pfn_on_msg))(context_data->callback_context, NOTICE_LOGOUT_LOCAL_SERVER, tcp_client->userid, 0, 0, 0);

					Thread_lock_mutex(context_data->
							  local_imsrv_lock);
					free(tcp_client);
					SL_Delete((SList) context_data->
						  connect_tcp_client_list, i,
						  NULL);
					Thread_unlock_mutex(context_data->
							    local_imsrv_lock);
					size--;	/*delete i size -- and i -- */
					i--;
					//if(context_data->callback.pfn_broken != NULL)
					//{
					//  (*(context_data->callback.pfn_broken))(context_data->callback_context,&server_data,"local client wait timeout");
					//}
				}

			}

			//return -1;
		} else if (ret > 0) {
			if (FD_ISSET(listenfd, &readfds)) {
				printf
				    ("local client is connecting,call accpet\n");
				connectfd =
				    accept(listenfd,
					   (struct sockaddr *)&client_addr,
					   &socklen);
				if (connectfd < 0) {
					Log(LOG_ERROR, 1, "Accept Failed!\n");
					continue;	//exit(1);
				}
				setnonblocking(connectfd);
				//setnonblocking(connectfd);
				tcp_client =
				    calloc(1, sizeof(struct tcp_socket_client));
				if (tcp_client == NULL) {
					Log(LOG_ERROR, 1,
					    "malloc tcp_client data fail\n");
					continue;
				}
				tcp_client->socket_fd = connectfd;
				tcp_client->time_out = 60;	//CLIENT_MAXLIVETIME;
				tcp_client->tcp_client_ddr[0] = 0;
				inet_ntop(AF_INET, &client_addr.sin_addr,
					  tcp_client->tcp_client_ddr,
					  sizeof(tcp_client->tcp_client_ddr));
				printf
				    ("local client connected, addr:%s,port:%d, socket:%d\n",
				     tcp_client->tcp_client_ddr,
				     ntohs(client_addr.sin_port), connectfd);
				//strcpy(tcp_client->tcp_client_ddr,inet_ntoa(client_addr.sin_addr));
				tcp_client->flag = 0;
				if (max_tcp_client < connectfd)
					max_tcp_client = connectfd;
				Thread_lock_mutex(context_data->
						  local_imsrv_lock);
				SL_Add((SList) context_data->
				       connect_tcp_client_list,
				       (void *)tcp_client);
				Thread_unlock_mutex(context_data->
						    local_imsrv_lock);

			}
#if 0
			else if (FD_ISSET(udp_broad_fd, &readfds)) {
				int user_data_len = sizeof(user_data);
				Log(LOG_PROTOCOL, 1,
				    "Now recevice broad cast data\n");
				memset(user_data, 0, sizeof(user_data));
				ret =
				    read_udp_broad_packet(udp_broad_fd, &cmd,
							  &seq, user_data,
							  &user_data_len,
							  &from_addr);
				if (ret == 0) {
					memset(userid, 0, sizeof(userid));
					//sscanf((char*)user_data,"%[^&]",userid);
					//eti_test_inner_print(__FILE__, __LINE__,"receive broad packet:%s\n userid:%s\n",  user_data, userid);
					//send_udp_flag  = 0;
					if (context_data->query_data.
					    pfn_on_query != NULL) {
						Log(LOG_PROTOCOL, -1,
						    "callback user query function \n");
						if (context_data->query_data.
						    pfn_on_query(context_data->
								 query_data.
								 context,
								 user_data,
								 user_data_len)
						    == 1) {
							//printf("Now is 1\n");
							send_udp_flag = 1;

						} else {
							//printf("Now is 0\n");
							send_udp_flag = 0;
						}
					} else {
						send_udp_flag = 1;
					}

					if (send_udp_flag == 1) {
						char broad_src_ip[64] = { 0 };
						memset(send_buff, 0,
						       sizeof(send_buff));
						inet_ntop(AF_INET,
							  &from_addr.sin_addr,
							  broad_src_ip,
							  sizeof(broad_src_ip));
						Log(LOG_PROTOCOL, -1,
						    "broad source addr is %s, send response\n",
						    broad_src_ip);
						/*tcp_client = malloc(sizeof(struct tcp_socket_client));
						   if(tcp_client == NULL)
						   {
						   Log(LOG_ERROR,1,"malloc tcp_client data fail\n");
						   continue;
						   }
						   tcp_client->socket_fd = -1;
						   strcpy(tcp_client->tcp_client_ddr,inet_ntoa(from_addr.sin_addr));
						   strcpy(tcp_client->userid,userid);
						   SL_Add((SList)context_data->connect_tcp_client_list,(void *)tcp_client); */

						ret =
						    build_udp_broad_revice_packet
						    (send_buff, seq,
						     context_data->userid);
						if (ret > 0) {
							et_send_udp_broad_revice_packet
							    (&from_addr,
							     send_buff, ret);
						}
					}

				}

			}
#endif
			else {
				int new_client_logined = 0;
				char new_client_UID[128];
				int new_client_index = 0;
				size =
				    SL_Size((SList) context_data->
					    connect_tcp_client_list);
				for (i = 1; i < size; i++) {
					rc = SL_Gethandle((SList) context_data->
							  connect_tcp_client_list,
							  i,
							  (ItemHandle *) &
							  tcp_client);
					if (rc != 0 || tcp_client == NULL) {
						Log(LOG_ERROR, 1,
						    "connect_tcp_client_list data is null please check\n");
						continue;
						//return -1;
					}

					if ((tcp_client->socket_fd > 0)
					    &&
					    (FD_ISSET
					     (tcp_client->socket_fd,
					      &client_exceptfds))) {
						printf
						    ("scoket:%d,exceptfds status\n ",
						     tcp_client->socket_fd);
						close(tcp_client->socket_fd);
						// (*(context_data->callback.pfn_on_msg))(context_data->callback_context, NOTICE_LOGOUT_LOCAL_SERVER, tcp_client->userid, 0, 0, 0);

						Thread_lock_mutex(context_data->
								  local_imsrv_lock);
						free(tcp_client);
						SL_Delete((SList) context_data->
							  connect_tcp_client_list,
							  i, NULL);
						Thread_unlock_mutex
						    (context_data->
						     local_imsrv_lock);
						size--;	/*delete i size -- and i -- */
						i--;

					} else if ((tcp_client->socket_fd > 0)
						   &&
						   (FD_ISSET
						    (tcp_client->socket_fd,
						     &readfds))) {
						memset(recv_buff, 0,
						       sizeof(recv_buff));
						ret =
						    tcp_recvn(tcp_client->
							      socket_fd,
							      recv_buff,
							      sizeof(recv_buff)
							      - 1);
						if (ret < 0) {
							printf
							    ("socket:%d, recv err:%d,tm:%lld, close it\n",
							     tcp_client->
							     socket_fd, ret,
							     eti_get_millisec
							     ());
							close(tcp_client->
							      socket_fd);
							//(*(context_data->callback.pfn_on_msg))(context_data->callback_context, NOTICE_LOGOUT_LOCAL_SERVER, tcp_client->userid, 0, 0, 0);
							/*server_data.port = tcp_client->port;
							   strcpy(server_data.addr,tcp_client->tcp_client_ddr);
							   server_data.type = MCU_SERVER_TYPE; 
							   strcpy(server_data.userid,tcp_client->userid); */
							Thread_lock_mutex
							    (context_data->
							     local_imsrv_lock);
							free(tcp_client);
							SL_Delete((SList)
								  context_data->
								  connect_tcp_client_list,
								  i, NULL);
							Thread_unlock_mutex
							    (context_data->
							     local_imsrv_lock);
							size--;	/*delete i size -- and i -- */
							i--;

						} else if (ret > 0) {
							printf
							    ("socket:%d, recv len:%d, tm:%lld\n",
							     tcp_client->
							     socket_fd, ret,
							     eti_get_millisec
							     ());
							//ret = parse_mqtt_packet(recv_buff,ret, &type,&ret_id,&payload,&payloadlen,&tmp_name,&name_len);
							int echolen =
							    tcp_sendn
							    (tcp_client->
							     socket_fd,
							     recv_buff, ret);
							if (echolen != ret) {
								printf
								    ("send echo failed, len:%d\n",
								     echolen);
							}

						} else if (ret == 0) {
							printf
							    ("socket:%d, recv nothing\n",
							     tcp_client->
							     socket_fd);
						}
					} else if ((tcp_client->socket_fd > 0)
						   &&
						   (FD_ISSET
						    (tcp_client->socket_fd,
						     &writefds))) {
						int wrs =
						    tcp_sendn(tcp_client->
							      socket_fd,
							      tcp_client->
							      need_write_ptr,
							      tcp_client->
							      need_write_size);
						if (wrs < 0) {
							printf
							    ("socket:%d, send error when select write ready:%d\n",
							     tcp_client->
							     socket_fd, errno);
						} else {
							tcp_client->
							    need_write_size -=
							    wrs;
							tcp_client->
							    need_write_ptr +=
							    wrs;
							if (tcp_client->
							    need_write_size)
								printf
								    ("socket:%d, send %d, leave %d, when select\n",
								     tcp_client->
								     socket_fd,
								     wrs,
								     tcp_client->
								     need_write_size);
						}
					}

				}

			}
		}

		if (retselect == 0
		    || eti_get_millisec() - auto_send_lasttm >=
		    context_data->local_server_auto_send_cycle) {
			auto_send_lasttm = eti_get_millisec();
			//keep_tcp_client_alive(context_data);
#define AUTO_SEND_LOCAL_TCPCLIENT
#ifdef AUTO_SEND_LOCAL_TCPCLIENT
			if (context_data->local_server_auto_send_cycle) {
				int send_size_per = sizeof(auto_send_buff);
				if (context_data->
				    local_server_auto_send_data_size > 0
				    && context_data->
				    local_server_auto_send_data_size <
				    send_size_per)
					send_size_per =
					    context_data->
					    local_server_auto_send_data_size;
				//int send_size_per = context_data->local_server_auto_send_data_size
				int send_atleast_alive = 0;
				size =
				    SL_Size((SList) context_data->
					    connect_tcp_client_list);
				for (i = 1; i < size; i++) {
					rc = SL_Gethandle((SList) context_data->
							  connect_tcp_client_list,
							  i,
							  (ItemHandle *) &
							  tcp_client);
					if (rc != 0 || tcp_client == NULL) {
						Log(LOG_ERROR, 1,
						    "connect_tcp_client_list data is null please check\n");
						continue;
						//return -1;
					}
					if (tcp_client->need_write_size == 0) {
						send_atleast_alive = 1;
						break;
					}
				}

				if (send_atleast_alive) {
					++auto_send_number;
					sprintf((char *)auto_send_buff, "%d",
						auto_send_number);
				}
				//printf("send:%s\n", auto_send_buff);

				size =
				    SL_Size((SList) context_data->
					    connect_tcp_client_list);
				for (i = 1; i < size; i++) {
					rc = SL_Gethandle((SList) context_data->
							  connect_tcp_client_list,
							  i,
							  (ItemHandle *) &
							  tcp_client);
					if (rc != 0 || tcp_client == NULL) {
						Log(LOG_ERROR, 1,
						    "connect_tcp_client_list data is null please check\n");
						continue;
						//return -1;
					}
					if (tcp_client->need_write_size == 0) {
						int thissnd =
						    tcp_sendn(tcp_client->
							      socket_fd,
							      auto_send_buff,
							      send_size_per);
						if (thissnd < 0) {
							printf
							    ("send auto data error, socket:%d,tm:%lld\n",
							     tcp_client->
							     socket_fd,
							     eti_get_millisec
							     ());
							close(tcp_client->
							      socket_fd);
							//(*(context_data->callback.pfn_on_msg))(context_data->callback_context, NOTICE_LOGOUT_LOCAL_SERVER, tcp_client->userid, 0, 0, 0);

							Thread_lock_mutex
							    (context_data->
							     local_imsrv_lock);
							free(tcp_client);
							SL_Delete((SList)
								  context_data->
								  connect_tcp_client_list,
								  i, NULL);
							Thread_unlock_mutex
							    (context_data->
							     local_imsrv_lock);
							size--;	/*delete i size -- and i -- */
							i--;
						} else {
							thissnd =
							    thissnd <
							    0 ? 0 : thissnd;
							if (thissnd <
							    send_size_per) {
								tcp_client->
								    need_write_size
								    =
								    send_size_per
								    - thissnd;
								tcp_client->
								    need_write_ptr
								    =
								    auto_send_buff
								    + thissnd;
								printf
								    ("socket:%d, send auto data %d, waitting write %d, tm:%lld\n ",
								     tcp_client->
								     socket_fd,
								     thissnd,
								     tcp_client->
								     need_write_size,
								     eti_get_millisec
								     ());
							} else {
								printf
								    ("socket:%d, send auto success %d, tm:%lld\n",
								     tcp_client->
								     socket_fd,
								     thissnd,
								     eti_get_millisec
								     ());
							}
						}
					}

				}
			}

#endif

		}
	}
	close(listenfd);
	//close(udp_broad_fd);
	size = SL_Size((SList) context_data->connect_tcp_client_list);
	for (i = 1; i < size; i++) {
		rc = SL_Gethandle((SList) context_data->connect_tcp_client_list,
				  i, (ItemHandle *) & tcp_client);
		if (rc != 0 || tcp_client == NULL) {
			Log(LOG_ERROR, 1,
			    "connect_tcp_client_list data is null please check\n");
			continue;
			//return -1;
		}
		close(tcp_client->socket_fd);
		Thread_lock_mutex(context_data->local_imsrv_lock);
		free(tcp_client);
		SL_Delete((SList) context_data->connect_tcp_client_list, i,
			  NULL);
		Thread_unlock_mutex(context_data->local_imsrv_lock);
		size--;		/*delete i size -- and i -- */
		i--;

	}

	context_data->server_thread_flag = 0;

	Log(LOG_PROTOCOL, -1, "local server stopped.\n");

	return 0;

 localsrv_err_exit:
	//if(udp_broad_fd > 0) close(udp_broad_fd);
	if (listenfd > 0)
		close(listenfd);
	/*if(context_data->query_data.on_failure){
	   (*context_data->query_data.on_failure)(context_data->query_data.context, &lsvrfail);
	   } */
	context_data->server_thread_flag = 0;
	return 0;
}

char *get_self_prog_name(char *argv0)
{
	char *ptr = strrchr(argv0, '/');
	if (ptr)
		return ++ptr;
	return argv0;
}

#define TEST_MAIN_SERVER

#ifdef TEST_MAIN_SERVER
int main(int argc, char *argv[])
{
	char kinput[2] = { 0, 0 };
	et_context client;
	et_context_data *context_data;
	int local_server_listen_port = 56603;
	int local_server_auto_send_cycle = 0;
	int local_server_auto_send_size = 1024;
	printf
	    ("tcp echo server tools\n\n");
	printf("Usage: ./%s local_port auto_cycle(ms) data_size \n\n when need server auto send data to client, please set argument: auto_cycle  data_size, so server send this size data by this time interval\nif set auto_cycle=0, then server don't auto send data, only echo data from cleint.\n",
	       get_self_prog_name(argv[0]));

	if (argc >= 2) {
		//strcpy(server,argv[1]);
		local_server_listen_port = atoi(argv[1]);
	}

	if (argc >= 3) {
		//strcpy(appkey,argv[2]);
		local_server_auto_send_cycle = atoi(argv[2]);
	}

	if (argc >= 4) {
		//strcpy(secretkey,argv[3]);
		local_server_auto_send_size = atoi(argv[3]);
	}

	et_create_context(&client);
	context_data = (et_context_data *) client;
	et_set_local_server_option(client, local_server_listen_port,
				   local_server_auto_send_cycle,
				   local_server_auto_send_size);

	context_data->server_thread_flag = 1;
	Thread_start(start_svr_thread, client);

	printf("wait message, when you hit key \'enter\', then exit\n");
	while ((!fgets(kinput, sizeof(kinput), stdin)) || (kinput[0] == 0))
		sleep(1);
	server_thread_stop_flag = 1;
	while (context_data->server_thread_flag) {
		printf("wait server thread end\n");
		sleep(1);
	}

	et_destroy_context(client);
	printf("server ended\n");

}

#endif
