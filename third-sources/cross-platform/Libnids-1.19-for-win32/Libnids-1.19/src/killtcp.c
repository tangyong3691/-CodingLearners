/*
  Copyright (c) 1999 Rafal Wojtczuk <nergal@avet.com.pl>. All rights reserved.
  See the file COPYING for license details.
*/
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#ifndef WIN32 /* Not supported. */
//#include <libnet.h>
#endif
#include "nids.h"
#include "tcp.h"
#include "util.h"

#if LIBNET_VER == 0
#ifndef WIN32 /* Not supported. */
//#include <libnet.h>
#endif
static int libnetsock = 0;

void nids_killtcp(struct tcp_stream *a_tcp)
{
	#ifndef WIN32 /* Not supported. */
    char buf[IP_H + TCP_H];

    if (libnetsock == 0)
	return;

    libnet_build_ip(TCP_H, 0, 12345, 0, 64, IPPROTO_TCP, a_tcp->addr.saddr,
		    a_tcp->addr.daddr, 0, 0, buf);
    libnet_build_tcp(a_tcp->addr.source, a_tcp->addr.dest,
		     a_tcp->client.seq + a_tcp->server.window / 2, 0,
		     0x4, 32000, 0, 0, 0, buf + IP_H);
    libnet_do_checksum(buf, IPPROTO_TCP, TCP_H);
    libnet_write_ip(libnetsock, buf, TCP_H + IP_H);

    libnet_build_ip(TCP_H, 0, 12345, 0, 64, IPPROTO_TCP, a_tcp->addr.daddr,
		    a_tcp->addr.saddr, 0, 0, buf);
    libnet_build_tcp(a_tcp->addr.dest, a_tcp->addr.source,
		     a_tcp->server.seq + a_tcp->client.window / 2, 0,
		     0x4, 32000, 0, 0, 0, buf + IP_H);
    libnet_do_checksum(buf, IPPROTO_TCP, TCP_H);
    libnet_write_ip(libnetsock, buf, TCP_H + IP_H);
	#endif
}

int raw_init()
{
	#ifndef WIN32 /* Not supported. */
    libnetsock = libnet_open_raw_sock(IPPROTO_RAW);
    if (libnetsock <= 0)
	return 0;
    else
	return 1;
	#else
	return 1;
	#endif
}
#elif LIBNET_VER == 1
	#ifndef WIN32 /* Not supported. */
	#include <libnet.h>
	#endif
static libnet_ptag_t tcp_tag = LIBNET_PTAG_INITIALIZER,
    ip_tag = LIBNET_PTAG_INITIALIZER;
static libnet_t *l = 0;
int raw_init()
{
	#ifndef WIN32 /* Not supported. */
    char errbuf[1024];
    l = libnet_init(LIBNET_RAW4,	/* injection type */
		    NULL,	/* network interface */
		    errbuf);	/* error buffer */

    if (!l) {
	printf("%s\n", errbuf);
	return 0;
    } else
	return 1;
	#else
		return 1;
	#endif
}

void nids_killtcp(struct tcp_stream *a_tcp)
{
	#ifndef WIN32 /* Not supported. */
    if (!l)
	return;
    tcp_tag = libnet_build_tcp(a_tcp->addr.source, a_tcp->addr.dest,
			       a_tcp->client.seq +
			       a_tcp->server.window / 2, 0, 0x4, 32000, 0,
			       0, LIBNET_TCP_H, NULL, 0, l, tcp_tag);
    ip_tag =
	libnet_build_ipv4(LIBNET_TCP_H + LIBNET_IPV4_H, 0, 12345, 0, 64,
			  IPPROTO_TCP, 0, a_tcp->addr.saddr,
			  a_tcp->addr.daddr, 0, 0, l, ip_tag);
    libnet_write(l);
    tcp_tag = libnet_build_tcp(a_tcp->addr.dest, a_tcp->addr.source,
			       a_tcp->server.seq +
			       a_tcp->client.window / 2, 0, 0x4, 32000, 0,
			       0, LIBNET_TCP_H, NULL, 0, l, tcp_tag);
    ip_tag =
	libnet_build_ipv4(LIBNET_TCP_H + LIBNET_IPV4_H, 0, 12345, 0, 64,
			  IPPROTO_TCP, 0, a_tcp->addr.daddr,
			  a_tcp->addr.saddr, 0, 0, l, ip_tag);
    libnet_write(l);
	#endif
}

#elif LIBNET_VER == -1
static int initialized = 0;
int raw_init()
{
	#ifndef WIN32 /* Not supported. */
    initialized = 1;
    return 1;
	#else
		return 1;
	#endif
}

void nids_killtcp(struct tcp_stream *a_tcp)
{
	#ifndef WIN32 /* Not supported. */
    (void)a_tcp;
    if (initialized)
	abort();
	#endif
}
#elif
	#error Something wrong with LIBNET_VER
#endif
