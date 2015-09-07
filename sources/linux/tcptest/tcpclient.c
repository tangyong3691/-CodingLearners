/* this is tcp client tool for test tcp data transmit
 *   support os: linux
 *   it based client_server mode, so please check server daemo start before test. 
 *
 *
 * Copyright (C) 2015 by Tang Yong.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*bug lists:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <semaphore.h>
#include <fcntl.h>
#include <resolv.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/timeb.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>


#define LOG_TRACE
#define MSGLEVEL 2

#define LOGINFO(fmt, args...)   do{ if( MSGLEVEL >= 1) printf ("[%d]%s(%d)%s: " fmt, getpid(), __FILE__, __LINE__,__func__, ## args); }while(0)
#define PRINTINFO(fmt, args...) do { if(MSGLEVEL >= 1) printf (fmt, ##args); }while(0)
#ifdef LOG_TRACE
#define LOGTRACE(fmt, args...)  do{ if( MSGLEVEL >= 2) printf ("[%d]%s(%d)%s: " fmt, getpid(),  __FILE__, __LINE__,__func__, ## args); }while(0)
#else
#define LOGTRACE(fmt, args...)
#endif

#define LOGERROR(fmt, args...) do{ if( MSGLEVEL >= 0) printf ("[%d]%s(%d)%s: ***err*** " fmt, getpid(), __FILE__, __LINE__,__func__, ## args); }while(0)

typedef struct cmdparm
{
	char ip[32] ;
	int port;
	int sport;
	int binary;
} cmdparm_t;

/*get current time in milliseconds*/
long long getTMmillisec(void)
{
	struct timeval tm;
	gettimeofday(&tm, 0);
	//LOGTRACE("TM sec %ld, usec %ld\n", tm.tv_sec, tm.tv_usec);
	return ((long long)tm.tv_sec) * 1000 + tm.tv_usec / 1000;
}

/*copy string with null ended*/
int strncpyz(char *dest, const char *src, int max)
{
	int len;
	if ( (!src) || (!dest) || max < 1 ) return 0;
	len = strlen(src);
	len = ( len < max - 1) ? len : max - 1;
	strncpy(dest, src, len);
	dest[len] = '\0';
	return len;
}

/*print help info*/
static void usage(char *name)
{
	printf("Usage: \n  %s [-i <ip>] [-p <port>] [-l <port>] [-a] messages \n\n"
	       "Options:\n"
	       "  -i , --ip , tcp server ip(default:192.168.1.10)\n"
	       "  -p,  --port, tcp server port(default:2000 \n"
	       "  -l, --local, local port(default: 30000)\n"
	       "  -a, --ascii, ascii message (default:hexadecimal digit)\n"
	       "  -h, --help    Print this message\n"
	       "messages format:\n"
	       "   when have option -a, ascii message be sent\n"
	       "   when no option -a, hexadecimal digit be sent\n"
	       "       hexadecimal digit format:\n"
	       "          header 9d9d (2bytes)\n"
	       "          length      (1bytes)\n"
	       "	  data        (user input)\n"
	       "	  checksum     (1bytes)\n"
	       "\n"
	       ,
	       name
	      );
}

/*parse cmd line arguments*/
static void parseArgs(int argc, char *argv[], char *name, cmdparm_t *envp)
{
	const char shortOptions[] = "i:p:l:ha";
	const struct option longOptions[] =
	{
		{"help", no_argument, NULL, 'h'},
		{"ip", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{"local", required_argument, NULL, 'l'},
		{"ascii", no_argument, NULL, 'a'},
		{0, 0, 0, 0}
	};
	int     index;
	int     c;

	for (;;)
	{
		c = getopt_long(argc, argv, shortOptions, longOptions, &index);

		if (c == -1)
		{
			break;
		}

		switch (c)
		{
		case 0:
			break;
		case 'i':
			strncpyz(envp->ip, optarg, 32);
			break;
		case 'p':
			envp->port = atoi(optarg);
			break;
		case 'l':
			envp->sport = atoi(optarg);
			break;
		case 'a':
			envp->binary = 0;
			break;
		case 'h':
			usage(name);
			exit(0);

		default:
			usage(name);
			exit(0);
		}
	}
}

#define CONNECT_SELECT_TIME_OUT_SEC 1
/*tcp connect establish
 * arguments:
 *    add:  dest ip
 *    port: dest port
 *    src_port: source port
 *    timeout_sec: timeout in seconds
 */
int TCPConnect(char *add, unsigned int port, int src_port, int timeout_sec)
{
	unsigned int ret;
	struct sockaddr_in tmp;
	unsigned int sfd;
	struct timeval 		OutTime;
	fd_set 				WriteFd;
	int   savefl = 0, retval = 0;
	int i = 0;
	struct sockaddr_in ipaddr;
	long long tm = getTMmillisec();
	if (timeout_sec <= 0)
	{
		LOGERROR("arg timeout_sec:%d\n", timeout_sec);
		return -1;
	}
	memset(&ipaddr, 0, sizeof(struct sockaddr_in));
	ipaddr.sin_family = AF_INET;
	ipaddr.sin_port = htons( src_port );
	ipaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(&tmp, 0, sizeof(struct sockaddr_in));
	tmp.sin_family = AF_INET;
	tmp.sin_port = htons(port);
	tmp.sin_addr.s_addr = inet_addr(add);

	memset(&OutTime, 0, sizeof(OutTime));
	OutTime.tv_usec = 0;
	OutTime.tv_sec = CONNECT_SELECT_TIME_OUT_SEC;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sfd)
	{
		LOGERROR("setup socket fail\n");
		return -1;
	}
	if (sfd == 0)
	{

		LOGERROR("**************************socket return 0:\n");
		return -1;
	}

	//LOGTRACE("socket clean   %d\n",sfd);
	fflush(stdout);

	if ( bind(sfd, &ipaddr, sizeof(ipaddr) ) < 0 )
	{
		LOGERROR("sock bind error\n");
	}

	savefl = fcntl(sfd, F_GETFL);

	fcntl(sfd,   F_SETFL,   savefl   |   O_NONBLOCK);
	if (connect(sfd, (struct sockaddr *)&tmp, sizeof(struct sockaddr_in)) == 0)
	{
		return   sfd;
	}

	if (errno != EINPROGRESS)
	{
		LOGERROR( "connect..\n ");
		close(sfd);
		return   -1;
	}

	//LOGTRACE("wait socket connect success\n");
	fflush(stdout);
	while ( 1 )
	{
		usleep(1);

		FD_ZERO(&WriteFd);
		FD_SET(sfd, &WriteFd);
		OutTime.tv_usec = 0;
		OutTime.tv_sec = CONNECT_SELECT_TIME_OUT_SEC;
		i++;

		if ( getTMmillisec() - tm > timeout_sec * 1000 )
		{
			LOGERROR( "timeout sfd:%d\n ", sfd );
			close(sfd);
			return   -1;
		}
		retval = select(sfd + 1, NULL, &WriteFd, NULL, &OutTime);
		if (retval == 0)
		{
			LOGTRACE("socket not ready to write: %d\n", i);
			OutTime.tv_sec = CONNECT_SELECT_TIME_OUT_SEC;
			if ( i > 10 )
				sleep(1);
			continue;
		}
		else if (retval == -1)
		{

			close(sfd);
			LOGERROR("select\n");
			return   -1;
		}
		else
		{
			//LOGTRACE("socket ready , select:%d\n", retval);
			break;
		}
	}
	fflush(stdout);
	retval = FD_ISSET(sfd, &WriteFd);
	//LOGTRACE("FD_ISSET retval:%d sfd:%d\n",retval,sfd);
	if (retval)            //||   FD_ISSET(SockFd,&wset))
	{
		int   error   =   0;
		socklen_t   len   =   sizeof   (error);
		if (getsockopt(sfd,   SOL_SOCKET,   SO_ERROR,   &error,   &len)   <   0)
		{
			LOGERROR( "getsockopt   fail,connected   fail   sfd:%d\n ", sfd);
			close(sfd);
			return   -1;
		}
		//below code in the block seems useless?
		if (error   ==   ETIMEDOUT)
		{
			LOGERROR( "connected   timeout  sfd:%d\n ", sfd);
			close(sfd);
			return -1;
		}

		if (error   ==   ECONNREFUSED)
		{
			LOGERROR( "connected   refused  sfd:%d\n ", sfd);
			close(sfd);
			return   -1;
		}
	}
	//LOGTRACE( "connected   ..   3      %d\n ",sfd);
	fcntl(sfd,   F_SETFL,   savefl);
	return sfd;
}


#define SEND_SELECT_TIMEOUT 1
#define SEND_SELECT_TIMEOUT_USEC 100
/*tcp data send
 * arguments:
 *    Sock:  socket
 *    SendBuf: data buffer of send
 *    nlen: send data lengths
 *    timeout_ms: timeout in milliseconds
 */
int Net_Send(unsigned int Sock, unsigned char *SendBuf, unsigned int nlen, int timeout_ms)
{
	int						sendlen = 0;
	fd_set 					WriteFd;
	struct timeval 				OutTime;
	int						ret = 0;
	int                                       counter = 0;
	int sval = 0;
	long long tm = getTMmillisec();
	memset(&OutTime, 0, sizeof(OutTime));
	OutTime.tv_usec = SEND_SELECT_TIMEOUT_USEC;
	OutTime.tv_sec = SEND_SELECT_TIMEOUT;

bb1:
	FD_ZERO(&WriteFd);
	FD_SET(Sock, &WriteFd);
	counter++;
	if ( getTMmillisec() - tm > timeout_ms )
	{
		LOGERROR("net send,  timeo ut\n");
		return -1;
	}

	ret = select( Sock + 1, NULL, &WriteFd, NULL, &OutTime);
	if ( ret < 0 )
	{
		OutTime.tv_usec = SEND_SELECT_TIMEOUT_USEC;
		OutTime.tv_sec = SEND_SELECT_TIMEOUT;
		LOGERROR("select send sock");
		return -1;
	}
	if ( ret == 0 )
	{
		OutTime.tv_usec = SEND_SELECT_TIMEOUT_USEC;
		OutTime.tv_sec = SEND_SELECT_TIMEOUT;
		usleep(1);
		goto bb1;
	}

	if (FD_ISSET( Sock, &WriteFd) > 0)
	{
		sendlen = send( Sock, SendBuf, nlen, 0);

		if (-1 == sendlen)
		{
			LOGERROR("send error");
			return -1;
		}

	}
	return sendlen;
}

#define SELECT_TIMEOUT_USEC 0
#define SELECT_TIMEOUT 1
/*tcp data receiv
 * arguments:
 *    Sock:  socket
 *    buf: data buffer of receive
 *    len: recive data lengths
 */
static int Net_Recv(unsigned int Sock, char *buf, int len)
{
	fd_set 				ReadFd;
	int 					recvlen = 0;
	struct timeval 		OutTime;
	int 					retvl = 0;

	if (len == 0)
	{
		LOGERROR("select recv len == 0");
		return -1;
	}
	if (len < 0)
	{
		LOGERROR("select recv len < 0,recvlen:%d", len);

		return -1;
	}

	memset(&OutTime, 0, sizeof(OutTime));
	OutTime.tv_usec = SELECT_TIMEOUT_USEC;
	OutTime.tv_sec = SELECT_TIMEOUT;

	FD_ZERO(&ReadFd);
	FD_SET(Sock, &ReadFd);

	retvl = select(Sock + 1, &ReadFd, NULL, NULL, &OutTime);
	if ( retvl > 0 )
	{
		if (FD_ISSET(Sock, &ReadFd) > 0)
		{
			recvlen = recv(Sock, buf, len, 0);
			if ( recvlen == 0 )
			{
				LOGERROR("recv return:%d", recvlen);
				return -1;
			}
			if ( recvlen < 0 )
			{
				LOGERROR("recv return < 0\n");
				return -1;
			}
			return recvlen;
		}
	}


	LOGERROR("retvl:%d\n", retvl);
	return -1;
}

#define RECEIVE_PER_ONCE 20
/*tcp data receiv in the special watting time
 * arguments:
 *    sock:  socket
 *    timeout_ms: timeout time in milliseconds
 *    buf: data buffer of receive
 *    maxlength: size of receive buffer
 */
int Net_recv_All(unsigned int sock, int timeout_ms, char *buf, int maxlength)
{
	int recvbytes = 0;
	int cnt = 0;
	int space = maxlength;
	int select_counter = 0;
	long long tm = getTMmillisec();
	int maxPer = ( maxlength < RECEIVE_PER_ONCE ) ? maxlength : RECEIVE_PER_ONCE;
	if (maxlength <= 0 || timeout_ms <= 0)
	{
		LOGERROR("arg maxlength:%d, timeout_ms:%d\n", maxlength, timeout_ms);
		return -1;
	}

	while (getTMmillisec() - tm < timeout_ms)
	{

		while (cnt == 0)
		{
			int	ret = Net_Recv(sock, buf + cnt, (space - cnt  < maxPer ? space - cnt : maxPer  ));
			if (ret < 0)
			{
				LOGERROR("recv return < 0, ret=%d\n", ret);
				return recvbytes ? recvbytes : ret;
			}
			else if ( ret == 0 )
			{
				select_counter++;
			}
			else
			{
				cnt += ret;
				select_counter = 0;
			}

			if ( select_counter >= 20 )
			{

				break;
			}

			if (ret != maxPer)
				usleep(0);
		}
		buf += cnt;
		recvbytes += cnt;
		space -= cnt;
		if (cnt != 0) break;
		cnt = 0;

		usleep(50 * 1000);
	}
	return recvbytes;
}

int main(int argc, char *argv[])
{
#define CONNECT_TM 5
#define MAX_REC_SZ 1024
#define MAX_SEND_SZ 256
	int sock;
	int ret;
	int cnt = 0;
	char *buf = 0;
	char receive_buf[MAX_REC_SZ + 1];
	char send_buf[MAX_SEND_SZ];
	cmdparm_t parm;

	char name[64];
	char *ppn;
	ppn = strrchr(argv[0], '/');
	strncpyz(name, ppn ? ppn + 1 : argv[0], 64);

	strcpy(parm.ip, "192.168.1.10");
	parm.port = 2000;
	parm.sport = 30000;
	parm.binary = 1;
	parseArgs(argc, argv, name, &parm);
	LOGTRACE("optind:%d, argc:%d\n", optind, argc);
	if (optind > argc - 1)
	{
		usage(name);
		exit(1);
	}
	else if (parm.binary)
	{
		/*check data isn't available formated hex string*/
		char ch;
		int ii = 0;
		do
		{
			ch = argv[optind][ii++];
			if ((ch) && (!isxdigit(ch)))
			{
				usage(name);
				exit(1);
			}
		}
		while (ch);
	}
	else
	{
	}
	sock = TCPConnect( parm.ip, parm.port, parm.sport, CONNECT_TM );
	if ( sock == -1 )
	{
		LOGERROR("Connect setup error");
		return -1;
	}
	LOGTRACE("TCPConnect success, sock:%d\n", sock);
	fflush(stdout);
	if (parm.binary)
#ifdef zigbee_test
	{
		/*digit format is:
		          header 9d9d (2bytes)
		          length      (1bytes)
			  data        (bytes = length - 4)
			  checksum     (1bytes)
		 */
		char *pt = send_buf;
		char dt[5] = {'0', 'x', 0, 0, 0};
		int i;
		*pt++ = 0x9d;
		*pt++ = 0x9d;
		pt++;
		cnt = 3;
		for (i = 0; i <  MAX_SEND_SZ - 4; i++)
		{
			char *p = &argv[optind][i * 2];
			if (p[0] == 0) break;
			if (p[1] == 0)
			{
				*(dt + 2) = '0';
				*(dt + 3) = *p;
			}
			else
			{
				strncpy(dt + 2, p, 2);
			}
			*pt++ = strtol(dt, 0, 16);
			cnt++;
			if (p[1] == 0) break;
		}
		/*fixme: checksum need calculate */
		*pt++ = 0;
		cnt++;
		send_buf[2] = cnt;
		LOGINFO("send data: ");
		for (i = 0; i < cnt; i++) PRINTINFO("%02x", (unsigned char)send_buf[i]);
		PRINTINFO("\n");

	}
#else
	{
		/*digit format is:
		          header 9d9d (2bytes)
		          length      (1bytes)
			  data        (bytes = length - 4)
			  checksum     (1bytes)
		 */
		char *pt = send_buf;
		char dt[5] = {'0', 'x', 0, 0, 0};
		int i;
		*pt++ = 0xbe;
                //*pt++ = 0x9d;
		pt++;
		cnt = 2;
		for (i = 0; i <  MAX_SEND_SZ - 4; i++)
		{
			char *p = &argv[optind][i * 2];
			if (p[0] == 0) break;
			if (p[1] == 0)
			{
				*(dt + 2) = '0';
				*(dt + 3) = *p;
			}
			else
			{
				strncpy(dt + 2, p, 2);
			}
			*pt++ = strtol(dt, 0, 16);
			cnt++;
			if (p[1] == 0) break;
		}
		/*fixme: checksum need calculate */
		*pt++ = 0;
		cnt++;
		send_buf[1] = cnt;
		LOGINFO("send data: ");
		for (i = 0; i < cnt; i++) PRINTINFO("%02x", (unsigned char)send_buf[i]);
		PRINTINFO("\n");

	}
#endif
	else
	{
		cnt = strncpyz(send_buf, argv[optind], MAX_SEND_SZ);
	}

	ret = Net_Send(sock, send_buf, cnt,  100 );
	if ( ret < 0 )
	{
		LOGERROR( "socket send error, ret=%d\n", ret );
		close(sock);
		return -1;
	}

	fflush(0);

	LOGTRACE("send success, bytes:%d\n", ret);

	cnt = Net_recv_All(sock, 100, receive_buf, MAX_REC_SZ);
	if (cnt > 0)
	{
		receive_buf[cnt] = 0;
		LOGINFO("reciev data , size = %d\n", cnt);
		if (parm.binary)
		{
			char *pt = receive_buf;
			LOGINFO("data: ");
			while (cnt--) PRINTINFO("%02x", (unsigned char)*pt++);
			PRINTINFO("\n");
		}
		else
		{
			LOGINFO("data: %s\n", receive_buf);
		}
	}
	else
	{
		LOGERROR("can't recieve any data!\n");
	}
	close(sock);
}
