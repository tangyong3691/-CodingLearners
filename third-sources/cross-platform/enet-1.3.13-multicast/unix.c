/**
 @file  unix.c
 @brief ENet Unix system specific functions
*/
#ifndef _WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/*add head for multicast*/
#include <net/if.h>

#define ENET_BUILDING_LIB 1
#include "enet/enet.h"

#ifdef __APPLE__
#ifdef HAS_POLL
#undef HAS_POLL
#endif
#ifndef HAS_FCNTL
#define HAS_FCNTL 1
#endif
#ifndef HAS_INET_PTON
#define HAS_INET_PTON 1
#endif
#ifndef HAS_INET_NTOP
#define HAS_INET_NTOP 1
#endif
#ifndef HAS_MSGHDR_FLAGS
#define HAS_MSGHDR_FLAGS 1
#endif
#ifndef HAS_SOCKLEN_T
#define HAS_SOCKLEN_T 1
#endif
#endif

#ifdef HAS_FCNTL
#include <fcntl.h>
#endif

#ifdef HAS_POLL
#include <sys/poll.h>
#endif

#ifndef HAS_SOCKLEN_T
typedef int socklen_t;
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

static enet_uint32 timeBase = 0;

int
enet_initialize (void)
{
    return 0;
}

void
enet_deinitialize (void)
{
}

enet_uint32
enet_host_random_seed (void)
{
    return (enet_uint32) time (NULL);
}

enet_uint32
enet_time_get (void)
{
    struct timeval timeVal;

    gettimeofday (& timeVal, NULL);

    return timeVal.tv_sec * 1000 + timeVal.tv_usec / 1000 - timeBase;
}

void
enet_time_set (enet_uint32 newTimeBase)
{
    struct timeval timeVal;

    gettimeofday (& timeVal, NULL);

    timeBase = timeVal.tv_sec * 1000 + timeVal.tv_usec / 1000 - newTimeBase;
}

int
enet_address_set_host (ENetAddress * address, const char * name)
{
    struct hostent * hostEntry = NULL;
#ifdef HAS_GETHOSTBYNAME_R
    struct hostent hostData;
    char buffer [2048];
    int errnum;

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
    gethostbyname_r (name, & hostData, buffer, sizeof (buffer), & hostEntry, & errnum);
#else
    hostEntry = gethostbyname_r (name, & hostData, buffer, sizeof (buffer), & errnum);
#endif
#else
    hostEntry = gethostbyname (name);
#endif

    if (hostEntry == NULL ||
        hostEntry -> h_addrtype != AF_INET)
    {
#ifdef HAS_INET_PTON
        if (! inet_pton (AF_INET, name, & address -> host))
#else
        if (! inet_aton (name, (struct in_addr *) & address -> host))
#endif
            return -1;
        return 0;
    }

    address -> host = * (enet_uint32 *) hostEntry -> h_addr_list [0];

    return 0;
}

int
enet_address_get_host_ip (const ENetAddress * address, char * name, size_t nameLength)
{
#ifdef HAS_INET_NTOP
    if (inet_ntop (AF_INET, & address -> host, name, nameLength) == NULL)
#else
    char * addr = inet_ntoa (* (struct in_addr *) & address -> host);
    if (addr != NULL)
    {
        size_t addrLen = strlen(addr);
        if (addrLen >= nameLength)
          return -1;
        memcpy (name, addr, addrLen + 1);
    }
    else
#endif
        return -1;
    return 0;
}

int
enet_address_get_host (const ENetAddress * address, char * name, size_t nameLength)
{
    struct in_addr in;
    struct hostent * hostEntry = NULL;
#ifdef HAS_GETHOSTBYADDR_R
    struct hostent hostData;
    char buffer [2048];
    int errnum;

    in.s_addr = address -> host;

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
    gethostbyaddr_r ((char *) & in, sizeof (struct in_addr), AF_INET, & hostData, buffer, sizeof (buffer), & hostEntry, & errnum);
#else
    hostEntry = gethostbyaddr_r ((char *) & in, sizeof (struct in_addr), AF_INET, & hostData, buffer, sizeof (buffer), & errnum);
#endif
#else
    in.s_addr = address -> host;

    hostEntry = gethostbyaddr ((char *) & in, sizeof (struct in_addr), AF_INET);
#endif

    if (hostEntry == NULL)
      return enet_address_get_host_ip (address, name, nameLength);
    else
    {
       size_t hostLen = strlen (hostEntry -> h_name);
       if (hostLen >= nameLength)
         return -1;
       memcpy (name, hostEntry -> h_name, hostLen + 1);
    }

    return 0;
}

int
enet_socket_bind (ENetSocket socket, const ENetAddress * address)
{
    struct sockaddr_in sin;

    memset (& sin, 0, sizeof (struct sockaddr_in));

    sin.sin_family = AF_INET;

    if (address != NULL)
    {
       sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
       sin.sin_addr.s_addr = address -> host;
    }
    else
    {
       sin.sin_port = 0;
       sin.sin_addr.s_addr = INADDR_ANY;
    }

    return bind (socket,
                 (struct sockaddr *) & sin,
                 sizeof (struct sockaddr_in));
}

int
enet_socket_get_address (ENetSocket socket, ENetAddress * address)
{
    struct sockaddr_in sin;
    socklen_t sinLength = sizeof (struct sockaddr_in);

    if (getsockname (socket, (struct sockaddr *) & sin, & sinLength) == -1)
      return -1;

    address -> host = (enet_uint32) sin.sin_addr.s_addr;
    address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);

    return 0;
}

int
enet_socket_listen (ENetSocket socket, int backlog)
{
    return listen (socket, backlog < 0 ? SOMAXCONN : backlog);
}

ENetSocket
enet_socket_create (ENetSocketType type)
{
    return socket (PF_INET, type == ENET_SOCKET_TYPE_DATAGRAM ? SOCK_DGRAM : SOCK_STREAM, 0);
}

int
enet_socket_set_option (ENetSocket socket, ENetSocketOption option, int value)
{
    int result = -1;
    switch (option)
    {
        case ENET_SOCKOPT_NONBLOCK:
#ifdef HAS_FCNTL
            result = fcntl (socket, F_SETFL, (value ? O_NONBLOCK : 0) | (fcntl (socket, F_GETFL) & ~O_NONBLOCK));
#else
            result = ioctl (socket, FIONBIO, & value);
#endif
            break;

        case ENET_SOCKOPT_BROADCAST:
            result = setsockopt (socket, SOL_SOCKET, SO_BROADCAST, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_REUSEADDR:
            result = setsockopt (socket, SOL_SOCKET, SO_REUSEADDR, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_RCVBUF:
            result = setsockopt (socket, SOL_SOCKET, SO_RCVBUF, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_SNDBUF:
            result = setsockopt (socket, SOL_SOCKET, SO_SNDBUF, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_RCVTIMEO:
        {
            struct timeval timeVal;
            timeVal.tv_sec = value / 1000;
            timeVal.tv_usec = (value % 1000) * 1000;
            result = setsockopt (socket, SOL_SOCKET, SO_RCVTIMEO, (char *) & timeVal, sizeof (struct timeval));
            break;
        }

        case ENET_SOCKOPT_SNDTIMEO:
        {
            struct timeval timeVal;
            timeVal.tv_sec = value / 1000;
            timeVal.tv_usec = (value % 1000) * 1000;
            result = setsockopt (socket, SOL_SOCKET, SO_SNDTIMEO, (char *) & timeVal, sizeof (struct timeval));
            break;
        }

        case ENET_SOCKOPT_NODELAY:
            result = setsockopt (socket, IPPROTO_TCP, TCP_NODELAY, (char *) & value, sizeof (int));
            break;

        default:
            break;
    }
    return result == -1 ? -1 : 0;
}

int
enet_socket_get_option (ENetSocket socket, ENetSocketOption option, int * value)
{
    int result = -1;
    socklen_t len;
    switch (option)
    {
        case ENET_SOCKOPT_ERROR:
            len = sizeof (int);
            result = getsockopt (socket, SOL_SOCKET, SO_ERROR, value, & len);
            break;

        default:
            break;
    }
    return result == -1 ? -1 : 0;
}

int
enet_socket_connect (ENetSocket socket, const ENetAddress * address)
{
    struct sockaddr_in sin;
    int result;

    memset (& sin, 0, sizeof (struct sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
    sin.sin_addr.s_addr = address -> host;

    result = connect (socket, (struct sockaddr *) & sin, sizeof (struct sockaddr_in));
    if (result == -1 && errno == EINPROGRESS)
      return 0;

    return result;
}

ENetSocket
enet_socket_accept (ENetSocket socket, ENetAddress * address)
{
    int result;
    struct sockaddr_in sin;
    socklen_t sinLength = sizeof (struct sockaddr_in);

    result = accept (socket,
                     address != NULL ? (struct sockaddr *) & sin : NULL,
                     address != NULL ? & sinLength : NULL);

    if (result == -1)
      return ENET_SOCKET_NULL;

    if (address != NULL)
    {
        address -> host = (enet_uint32) sin.sin_addr.s_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);
    }

    return result;
}

int
enet_socket_shutdown (ENetSocket socket, ENetSocketShutdown how)
{
    return shutdown (socket, (int) how);
}

void
enet_socket_destroy (ENetSocket socket)
{
    if (socket != -1)
      close (socket);
}

int
enet_socket_send (ENetSocket socket,
                  const ENetAddress * address,
                  const ENetBuffer * buffers,
                  size_t bufferCount)
{
    struct msghdr msgHdr;
    struct sockaddr_in sin;
    int sentLength;

    memset (& msgHdr, 0, sizeof (struct msghdr));

    if (address != NULL)
    {
        memset (& sin, 0, sizeof (struct sockaddr_in));

        sin.sin_family = AF_INET;
        sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
        sin.sin_addr.s_addr = address -> host;

        msgHdr.msg_name = & sin;
        msgHdr.msg_namelen = sizeof (struct sockaddr_in);
    }

    msgHdr.msg_iov = (struct iovec *) buffers;
    msgHdr.msg_iovlen = bufferCount;

    sentLength = sendmsg (socket, & msgHdr, MSG_NOSIGNAL);

    if (sentLength == -1)
    {
       if (errno == EWOULDBLOCK)
         return 0;

       return -1;
    }

    return sentLength;
}

int
enet_socket_receive (ENetSocket socket,
                     ENetAddress * address,
                     ENetBuffer * buffers,
                     size_t bufferCount)
{
    struct msghdr msgHdr;
    struct sockaddr_in sin;
    int recvLength;

    memset (& msgHdr, 0, sizeof (struct msghdr));

    if (address != NULL)
    {
        msgHdr.msg_name = & sin;
        msgHdr.msg_namelen = sizeof (struct sockaddr_in);
    }

    msgHdr.msg_iov = (struct iovec *) buffers;
    msgHdr.msg_iovlen = bufferCount;

    recvLength = recvmsg (socket, & msgHdr, MSG_NOSIGNAL);

    if (recvLength == -1)
    {
       if (errno == EWOULDBLOCK)
         return 0;

       return -1;
    }

#ifdef HAS_MSGHDR_FLAGS
    if (msgHdr.msg_flags & MSG_TRUNC)
      return -1;
#endif

    if (address != NULL)
    {
        address -> host = (enet_uint32) sin.sin_addr.s_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);
    }

    return recvLength;
}

int
enet_socketset_select (ENetSocket maxSocket, ENetSocketSet * readSet, ENetSocketSet * writeSet, enet_uint32 timeout)
{
    struct timeval timeVal;

    timeVal.tv_sec = timeout / 1000;
    timeVal.tv_usec = (timeout % 1000) * 1000;

    return select (maxSocket + 1, readSet, writeSet, NULL, & timeVal);
}

int
enet_socket_wait (ENetSocket socket, enet_uint32 * condition, enet_uint32 timeout)
{
#ifdef HAS_POLL
    struct pollfd pollSocket;
    int pollCount;

    pollSocket.fd = socket;
    pollSocket.events = 0;

    if (* condition & ENET_SOCKET_WAIT_SEND)
      pollSocket.events |= POLLOUT;

    if (* condition & ENET_SOCKET_WAIT_RECEIVE)
      pollSocket.events |= POLLIN;

    pollCount = poll (& pollSocket, 1, timeout);

    if (pollCount < 0)
    {
        if (errno == EINTR && * condition & ENET_SOCKET_WAIT_INTERRUPT)
        {
            * condition = ENET_SOCKET_WAIT_INTERRUPT;

            return 0;
        }

        return -1;
    }

    * condition = ENET_SOCKET_WAIT_NONE;

    if (pollCount == 0)
      return 0;

    if (pollSocket.revents & POLLOUT)
      * condition |= ENET_SOCKET_WAIT_SEND;

    if (pollSocket.revents & POLLIN)
      * condition |= ENET_SOCKET_WAIT_RECEIVE;

    return 0;
#else
    fd_set readSet, writeSet;
    struct timeval timeVal;
    int selectCount;

    timeVal.tv_sec = timeout / 1000;
    timeVal.tv_usec = (timeout % 1000) * 1000;

    FD_ZERO (& readSet);
    FD_ZERO (& writeSet);

    if (* condition & ENET_SOCKET_WAIT_SEND)
      FD_SET (socket, & writeSet);

    if (* condition & ENET_SOCKET_WAIT_RECEIVE)
      FD_SET (socket, & readSet);

    selectCount = select (socket + 1, & readSet, & writeSet, NULL, & timeVal);

    if (selectCount < 0)
    {
        if (errno == EINTR && * condition & ENET_SOCKET_WAIT_INTERRUPT)
        {
            * condition = ENET_SOCKET_WAIT_INTERRUPT;

            return 0;
        }

        return -1;
    }

    * condition = ENET_SOCKET_WAIT_NONE;

    if (selectCount == 0)
      return 0;

    if (FD_ISSET (socket, & writeSet))
      * condition |= ENET_SOCKET_WAIT_SEND;

    if (FD_ISSET (socket, & readSet))
      * condition |= ENET_SOCKET_WAIT_RECEIVE;

    return 0;
#endif
}


/*for multicast, add function below here*/


#define IFI_NAME    16          /* same as IFNAMSIZ in <net/if.h> */
#define IFI_HADDR    8          /* allow for 64-bit EUI-64 in future */
#define IFI_ALIAS   1           /* ifi_addr is an alias */
#define NOT_HAVE_SA_LEN
#define GET_SA_LEN(X) (((struct sockaddr*)&(X))->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr))

struct ifi_info {
  char    ifi_name[IFI_NAME];   /* interface name, null terminated */
  u_char  ifi_haddr[IFI_HADDR]; /* hardware address */
  u_short ifi_hlen;             /* #bytes in hardware address: 0, 6, 8 */
  short   ifi_flags;            /* IFF_xxx constants from <net/if.h> */
  short   ifi_myflags;          /* our own IFI_xxx flags */
  int     ifi_index;            /* interface index */
  struct sockaddr  *ifi_addr;   /* primary address */
  struct sockaddr  *ifi_netmask;
  struct sockaddr  *ifi_brdaddr;/* broadcast address */
  struct sockaddr  *ifi_dstaddr;/* destination address */
  struct ifi_info  *ifi_next;   /* next of these structures */
};

struct ifi_info *get_ifi_info(int family, int doaliases)
{
    int                 junk;
    struct ifi_info     *ifi, *ifihead, **ifipnext;
    int                 sockfd, sockf6, len, lastlen, flags, myflags;
#ifdef NOT_HAVE_IF_NAMETOINDEX
    int                 index = 200;
#endif
    char                *ptr, *buf, lastname[IFNAMSIZ], *cptr;
    struct ifconf       ifc;
    struct ifreq        *ifr, ifrcopy;
    struct sockaddr_in  *sinptr;

#if defined(AF_INET6) && HAVE_IPV6
    struct sockaddr_in6 *sinptr6;
#endif

#if defined(AF_INET6) && HAVE_IPV6 && HAVE_LINUX
 if(family == AF_INET6) return get_ifi_info_linuxv6(family, doaliases);
#endif

	sockfd = -1;
    sockf6 = -1;
    buf = NULL;
    ifihead = NULL;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        goto gotError;
    }

    lastlen = 0;
    len = 100 * sizeof(struct ifreq);   /* initial buffer size guess */
    for ( ; ; ) {
        buf = (char*)malloc(len);
        if (buf == NULL) {
            goto gotError;
        }
        ifc.ifc_len = len;
        ifc.ifc_buf = buf;
        if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
            if (errno != EINVAL || lastlen != 0) {
                goto gotError;
            }
        } else {
            if (ifc.ifc_len == lastlen)
                break;      /* success, len has not changed */
            lastlen = ifc.ifc_len;
        }
        len += 10 * sizeof(struct ifreq);   /* increment */
        free(buf);
    }
    ifihead = NULL;
    ifipnext = &ifihead;
    lastname[0] = 0;
/* end get_ifi_info1 */

/* include get_ifi_info2 */
    for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
        ifr = (struct ifreq *) ptr;

        /* Advance to next one in buffer */
        if (sizeof(struct ifreq) > sizeof(ifr->ifr_name) + GET_SA_LEN(ifr->ifr_addr))
            ptr += sizeof(struct ifreq);
        else
            ptr += sizeof(ifr->ifr_name) + GET_SA_LEN(ifr->ifr_addr);

//      fprintf(stderr, "intf %p name=%s AF=%d\n", index, ifr->ifr_name, ifr->ifr_addr.sa_family);

        if (ifr->ifr_addr.sa_family != family)
            continue;   /* ignore if not desired address family */

        myflags = 0;
        if ( (cptr = strchr(ifr->ifr_name, ':')) != NULL)
            *cptr = 0;      /* replace colon will null */
        if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0) {
            if (doaliases == 0)
                continue;   /* already processed this interface */
            myflags = IFI_ALIAS;
        }
        memcpy(lastname, ifr->ifr_name, IFNAMSIZ);

        ifrcopy = *ifr;
        if (ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy) < 0) {
            goto gotError;
        }

        flags = ifrcopy.ifr_flags;
        if ((flags & IFF_UP) == 0)
            continue;   /* ignore if interface not up */

        ifi = (struct ifi_info*)calloc(1, sizeof(struct ifi_info));
        if (ifi == NULL) {
            goto gotError;
        }
        *ifipnext = ifi;            /* prev points to this new one */
        ifipnext = &ifi->ifi_next;  /* pointer to next one goes here */

        ifi->ifi_flags = flags;     /* IFF_xxx values */
        ifi->ifi_myflags = myflags; /* IFI_xxx values */
#ifndef NOT_HAVE_IF_NAMETOINDEX
        ifi->ifi_index = if_nametoindex(ifr->ifr_name);
#else
        ifrcopy = *ifr;
#ifdef SIOCGIFINDEX
		if ( 0 >= ioctl(sockfd, SIOCGIFINDEX, &ifrcopy))
            ifi->ifi_index = ifrcopy.ifr_index;
        else
#endif
            ifi->ifi_index = index++;	/* SIOCGIFINDEX is broken on Solaris 2.5ish, so fake it */
#endif
        memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
        ifi->ifi_name[IFI_NAME-1] = '\0';
/* end get_ifi_info2 */
/* include get_ifi_info3 */
        switch (ifr->ifr_addr.sa_family) {
        case AF_INET:
            sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
            if (ifi->ifi_addr == NULL) {
                ifi->ifi_addr = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in));
                if (ifi->ifi_addr == NULL) {
                    goto gotError;
                }
                memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));

#ifdef  SIOCGIFNETMASK
				if (ioctl(sockfd, SIOCGIFNETMASK, &ifrcopy) < 0) goto gotError;
				ifi->ifi_netmask = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in));
				if (ifi->ifi_netmask == NULL) goto gotError;
				sinptr = (struct sockaddr_in *) &ifrcopy.ifr_addr;
				/* The BSD ioctls (including Mac OS X) stick some weird values in for sin_len and sin_family */
#ifndef NOT_HAVE_SA_LEN
				sinptr->sin_len    = sizeof(struct sockaddr_in);
#endif
				sinptr->sin_family = AF_INET;
				memcpy(ifi->ifi_netmask, sinptr, sizeof(struct sockaddr_in));
#endif

#ifdef  SIOCGIFBRDADDR
                if (flags & IFF_BROADCAST) {
                    if (ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy) < 0) {
                        goto gotError;
                    }
                    sinptr = (struct sockaddr_in *) &ifrcopy.ifr_broadaddr;
					/* The BSD ioctls (including Mac OS X) stick some weird values in for sin_len and sin_family */
#ifndef NOT_HAVE_SA_LEN
					sinptr->sin_len    = sizeof( struct sockaddr_in );
#endif
					sinptr->sin_family = AF_INET;
                    ifi->ifi_brdaddr = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in));
                    if (ifi->ifi_brdaddr == NULL) {
                        goto gotError;
                    }
                    memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
                }
#endif

#ifdef  SIOCGIFDSTADDR
                if (flags & IFF_POINTOPOINT) {
                    if (ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy) < 0) {
                        goto gotError;
                    }
                    sinptr = (struct sockaddr_in *) &ifrcopy.ifr_dstaddr;
                    /* The BSD ioctls (including Mac OS X) stick some weird values in for sin_len and sin_family */
#ifndef NOT_HAVE_SA_LEN
					sinptr->sin_len    = sizeof( struct sockaddr_in );
#endif
					sinptr->sin_family = AF_INET;
                    ifi->ifi_dstaddr = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in));
                    if (ifi->ifi_dstaddr == NULL) {
                        goto gotError;
                    }
                    memcpy(ifi->ifi_dstaddr, sinptr, sizeof(struct sockaddr_in));
                }
#endif
            }
            break;

#if defined(AF_INET6) && HAVE_IPV6
        case AF_INET6:
            sinptr6 = (struct sockaddr_in6 *) &ifr->ifr_addr;
            if (ifi->ifi_addr == NULL) {
                ifi->ifi_addr = calloc(1, sizeof(struct sockaddr_in6));
                if (ifi->ifi_addr == NULL) {
                    goto gotError;
                }

                /* Some platforms (*BSD) inject the prefix in IPv6LL addresses */
                /* We need to strip that out */
                if (IN6_IS_ADDR_LINKLOCAL(&sinptr6->sin6_addr))
                	sinptr6->sin6_addr.s6_addr[2] = sinptr6->sin6_addr.s6_addr[3] = 0;
                memcpy(ifi->ifi_addr, sinptr6, sizeof(struct sockaddr_in6));

#ifdef  SIOCGIFNETMASK_IN6
				{
				struct in6_ifreq ifr6;
				if (sockf6 == -1)
					sockf6 = socket(AF_INET6, SOCK_DGRAM, 0);
				memset(&ifr6, 0, sizeof(ifr6));
				memcpy(&ifr6.ifr_name,           &ifr->ifr_name, sizeof(ifr6.ifr_name          ));
				memcpy(&ifr6.ifr_ifru.ifru_addr, &ifr->ifr_addr, sizeof(ifr6.ifr_ifru.ifru_addr));
				if (ioctl(sockf6, SIOCGIFNETMASK_IN6, &ifr6) < 0) goto gotError;
				ifi->ifi_netmask = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in6));
				if (ifi->ifi_netmask == NULL) goto gotError;
				sinptr6 = (struct sockaddr_in6 *) &ifr6.ifr_ifru.ifru_addr;
				memcpy(ifi->ifi_netmask, sinptr6, sizeof(struct sockaddr_in6));
				}
#endif
            }
            break;
#endif

        default:
            break;
        }
    }
    goto done;

gotError:
    if (ifihead != NULL) {
        free_ifi_info(ifihead);
        ifihead = NULL;
    }

done:
    if (buf != NULL) {
        free(buf);
    }
    if (sockfd != -1) {
        junk = close(sockfd);
        //assert(junk == 0);
    }
    if (sockf6 != -1) {
        junk = close(sockf6);
        //assert(junk == 0);
    }
    return(ifihead);    /* pointer to first structure in linked list */
}
/* end get_ifi_info3 */

/* include free_ifi_info */
void
free_ifi_info(struct ifi_info *ifihead)
{
    struct ifi_info *ifi, *ifinext;

    for (ifi = ifihead; ifi != NULL; ifi = ifinext) {
        if (ifi->ifi_addr != NULL)
            free(ifi->ifi_addr);
        if (ifi->ifi_netmask != NULL)
            free(ifi->ifi_netmask);
        if (ifi->ifi_brdaddr != NULL)
            free(ifi->ifi_brdaddr);
        if (ifi->ifi_dstaddr != NULL)
            free(ifi->ifi_dstaddr);
        ifinext = ifi->ifi_next;    /* can't fetch ifi_next after free() */
        free(ifi);                  /* the ifi_info{} itself */
    }
}
/* end free_ifi_info */

/* get active ip list for all net if
 * parm:
 *   count [in, out]  [in] ptr of max size of iplist buffer [out] fill numbers of ip
 *   iplist [out]  buffer of ip lists
*/
int
enet_socket_get_ipv4(int *count, unsigned *iplist)
{
	int             err            = 0;
	struct ifi_info *intfList      = get_ifi_info(AF_INET, 0);
	struct ifi_info *firstLoopback = NULL;
	int bufSize = count ? *count : 0;
	int counts = 0;

	if (count == 0 && iplist == 0) err = -1;
	if (bufSize != 0 && iplist == 0) err = -1;
	if (intfList == NULL) err = -1;

#if HAVE_IPV6
	if (err == 0)		/* Link the IPv6 list to the end of the IPv4 list */
		{
		struct ifi_info **p = &intfList;
		while (*p) p = &(*p)->ifi_next;
		*p = get_ifi_info(AF_INET6, mDNStrue);
		}
#endif

	if (err == 0)
		{
		struct ifi_info *i = intfList;
		while (i)
			{
			if (     ((i->ifi_addr->sa_family == AF_INET)
#if HAVE_IPV6
					  || (i->ifi_addr->sa_family == AF_INET6)
#endif
				) &&  (i->ifi_flags & IFF_UP) && !(i->ifi_flags & IFF_POINTOPOINT))
				{
				if (i->ifi_flags & IFF_LOOPBACK)
					{
					if (firstLoopback == NULL)
						firstLoopback = i;
					}
				else
					{
						if(i->ifi_addr->sa_family == AF_INET)
						{

							if(counts < bufSize)
							{
								iplist[counts] = ((struct sockaddr_in*)i->ifi_addr)->sin_addr.s_addr;
							}
							counts++;
						}
					/*if (SetupOneInterface(m, i->ifi_addr, i->ifi_netmask, i->ifi_name, i->ifi_index) == 0)
						if (i->ifi_addr->sa_family == AF_INET)
							foundav4 = mDNStrue;*/
					}
				}
			i = i->ifi_next;
			}


		}

	// Clean up.
	if (intfList != NULL) free_ifi_info(intfList);
	if(count) *count = counts;
	if((counts > bufSize && bufSize) || counts == 0) err = -1;
	return err;
}

int
enet_socket_set_option_ex (ENetSocket socket, ENetSocketOption option, void *parm)
{
   int result = -1;
    switch (option)
    {
        case ENET_SOCKOPT_MULTICAST:
		{
			struct ip_mreq mreq;
			struct sockaddr_in *addr = (struct sockaddr_in *)parm;
        	memset(&mreq, 0, sizeof(struct ip_mreq));
        	mreq.imr_interface.s_addr = addr->sin_addr.s_addr;
        	mreq.imr_multiaddr.s_addr = addr[1].sin_addr.s_addr;
        	//setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value));
        	//setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
        	//setsockopt(socket, IPPROTO_IP, IP_PKTINFO, &value, sizeof(value));
        	result = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq));
		    //setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &value, sizeof(value));
		}
			break;

		case ENET_SOCKOPT_REUSEPORT:
#if defined(SO_REUSEPORT)
		{
			int *on = (int *)parm;
			result = setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, on, sizeof(*on));
		}
#endif
			break;

		case ENET_SOCKOPT_PKTINFO:
		{
			int *on = (int *)parm;
			result = setsockopt(socket, IPPROTO_IP, IP_PKTINFO, on, sizeof(*on));
		}
			break;

		case ENET_SOCKOPT_MULTICAST_IF:
		{
			struct sockaddr_in *addr = (struct sockaddr_in *)parm;
			result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_IF, &addr->sin_addr, sizeof(struct in_addr));
		}
			break;

		case ENET_SOCKOPT_MULTICAST_TTL:
		{
			unsigned char *ttl = (unsigned char *)parm;
			result = setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, ttl, sizeof(*ttl));
		}
			break;

        default:
            break;
    }
    return result == -1 ? -1 : 0;
}

#endif

