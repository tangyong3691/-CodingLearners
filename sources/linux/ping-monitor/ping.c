#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <strings.h>
#include <string.h>


#include "ping.h"
#include "const.h"

int get_icmp_addr(const char *ip, struct sockaddr_in *svr_addr);
int send_ping(int fd, unsigned short pack_no, struct sockaddr_in *svr_addr);
static char *set_icmp_pkg(unsigned short pack_no, char *pkg, int pkg_len);
int recv_ping(int fd, unsigned short pack_no, struct sockaddr_in *svr_addr, char *recv_data, int recv_data_len, int timeout);
int fd_select_wait(int fd, int is_read, int is_write, int is_except, int time_out);
unsigned short check_sum(unsigned short *addr, int len);




int ping_test(char *url)
{ 
	int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	unsigned short pkg_no = 65520;
	struct sockaddr_in svr_addr;
	char buf[1024];
	int ping_ret = 0;

	if(!get_icmp_addr(url, &svr_addr))
	{
		ping_ret = PING_GET_ADDRESS_ERROR;
		return PING_GET_ADDRESS_ERROR;
	}
	if(!send_ping(fd, pkg_no, &svr_addr))
	{
		printf("PING_SEND_ERROR\n");
		ping_ret = PING_SEND_ERROR;
		return PING_SEND_ERROR;
	}
	if((ping_ret = recv_ping(fd, pkg_no, &svr_addr, buf, sizeof(buf), 1000)) < 0)
	{
		printf("PING_RECV_ERROR\n");
		ping_ret = PING_RECV_ERROR;
		//g_critical("%s dev[%s], recv ping packet error.", __func__, up_thread_name[card_type]);
		return PING_RECV_ERROR;
	}

	close(fd);
    return ping_ret;
}

int get_icmp_addr(const char *ip, struct sockaddr_in *svr_addr)
{
   struct hostent *host = NULL;

    bzero(svr_addr, sizeof(*svr_addr));
    svr_addr->sin_family = AF_INET;

    //�ж��������Ƿ���IP��ַ
    if(inet_addr(ip) == INADDR_NONE)
    {
        if((host = gethostbyname(ip)) == NULL) //��������
        {
           return FALSE;
        }
        memcpy((char *)&svr_addr->sin_addr, host->h_addr, host->h_length);
    }
    else
    {
        svr_addr->sin_addr.s_addr = inet_addr(ip); //��IP ��ַ
    }

    return TRUE;
}

int send_ping(int fd, unsigned short pack_no, struct sockaddr_in *svr_addr)
{
    struct icmp send_data;
    set_icmp_pkg(pack_no, (char *)&send_data, sizeof(send_data));

    //�������ݱ�
    if(sendto(fd, (char *)&send_data, sizeof(send_data), 0,
              (struct sockaddr *)svr_addr, sizeof(*svr_addr)) < 0)
    {
        return FALSE;
    }

    return TRUE;
}

//����ICMP���ݰ�
static char *set_icmp_pkg(unsigned short pack_no, char *pkg, int pkg_len)
{
    struct icmp *icmp_buf = (struct icmp*)pkg;
    struct timeval *tval = (struct timeval *)icmp_buf->icmp_data;

    icmp_buf->icmp_type = ICMP_ECHO; //ICMP_ECHO���͵����ͺ�Ϊ0
    icmp_buf->icmp_code = 0;
    icmp_buf->icmp_cksum = 0;
    icmp_buf->icmp_seq = pack_no; //���͵����ݱ����
    icmp_buf->icmp_id = getpid();
    gettimeofday(tval, NULL); //��¼����ʱ��
    icmp_buf->icmp_cksum = check_sum((unsigned short *)icmp_buf, pkg_len);   //У���㷨

    return pkg;
}

int recv_ping(int fd, unsigned short pack_no, struct sockaddr_in *svr_addr, char *recv_data, int recv_data_len, int timeout)
{
    while (TRUE)
    {
        int ret = fd_select_wait(fd, TRUE, FALSE, FALSE, timeout);
        if(ret != 0x001)
        {
            return -1; //��ʱ
        }

        socklen_t fromlen;

        int recv_len = recvfrom(fd, recv_data, recv_data_len,0,
                                 (struct sockaddr *)svr_addr, &fromlen);
        //�������ݱ�
        if(recv_len < 0)
        {
            return -1;
        }

        struct timeval tvrecv;
        gettimeofday(&tvrecv,NULL); //��¼����ʱ��

        int iphdrlen;       //ipͷ����
        struct ip *ip;
        struct icmp *icmp;
        struct timeval *tvsend;

        ip = (struct ip *)recv_data;
        iphdrlen = ip->ip_hl << 2; //��IP����ͷ���ȣ���IP��ͷ���ȳ�4
        icmp = (struct icmp *)(recv_data + iphdrlen); //Խ��IPͷ��ָ��ICMP��ͷ
        recv_len -= iphdrlen; //ICMP��ͷ�����ݱ����ܳ���
        if(recv_len < 8)      //С��ICMP��ͷ�ĳ����򲻺���
        {
            return -1;
        }

        //ȷ�������յ���������ICMP�Ļ�Ӧ
        if((icmp->icmp_type == ICMP_ECHOREPLY) &&
           (icmp->icmp_id == getpid()) &&
           (icmp->icmp_seq == pack_no))
        {
            tvsend = (struct timeval *)icmp->icmp_data;
            unsigned long long rv_ms = tvrecv.tv_sec * 1000 + tvrecv.tv_usec/1000;
            unsigned long long tv_ms = tvsend->tv_sec * 1000 + tvsend->tv_usec/1000;

            return rv_ms - tv_ms;
        }
    }

    return 0;
}

int fd_select_wait(int fd, int is_read, int is_write, int is_except, int time_out)
{
    int ret = -1;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = time_out * 1000;     //select��ʱʱ��

    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);
    if(is_read) FD_SET(fd, &rfds);
    if(is_write) FD_SET(fd, &wfds);
    if(is_except) FD_SET(fd, &efds);

    int select_ret = select(fd + 1, &rfds, &wfds, &efds, &tv);
    if(0 == select_ret)
    {
        return 0;             //��ʱ
    }

    if(select_ret < 0)
    {
        return -1;
    }

    ret = 0;
    if(FD_ISSET(fd, &rfds)) ret |= 0x001;
    if(FD_ISSET(fd, &wfds)) ret |= 0x010;
    if(FD_ISSET(fd, &efds)) ret |= 0x100;

    return ret;
}

//������㷨
unsigned short check_sum(unsigned short *addr, int len)
{
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short check_sum = 0;

    while(nleft>1)       //ICMP��ͷ���֣�2�ֽڣ�Ϊ��λ�ۼ�
    {
        sum += *w++;
        nleft -= 2;
    }

    if(nleft == 1)      //ICMPΪ�����ֽ�ʱ��ת�����һ���ֽڣ������ۼ�
    {
        *(unsigned char *)(&check_sum) = *(unsigned char *)w;
        sum += check_sum;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    check_sum = ~sum;   //ȡ���õ�У���

    return check_sum;
}

