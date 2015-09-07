/**
 @file  win32.c
 @brief ENet Win32 system specific functions
*/
#ifdef WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winsock.h>
#include <time.h>
#define ENET_BUILDING_LIB 1
#include "enet/enet.h"

struct ifaddrs
{
	struct ifaddrs *	ifa_next;
	char *				ifa_name;
	u_int				ifa_flags;
	u_int               real_flags; /*1: is physical Ethernet Adapter, 0: virtual*/
	struct sockaddr	*	ifa_addr;
	struct sockaddr	*	ifa_netmask;
	struct sockaddr	*	ifa_broadaddr;
	struct sockaddr	*	ifa_dstaddr;
	BOOL				ifa_dhcpEnabled;
	time_t				ifa_dhcpLeaseExpires;
	unsigned char				ifa_womp;
	void *				ifa_data;

	struct
	{
		unsigned int		index;

	}	ifa_extra;
};

#define require_action(X, LABEL, ACTION)   \
         do { \
		        if ( !(X) ) \
				{ \
				    {ACTION;} \
					goto LABEL; \
				} \
			} while( 0 )

static enet_uint32 timeBase = 0;

int
enet_initialize (void)
{
    WORD versionRequested = MAKEWORD (1, 1);
    WSADATA wsaData;

    if (WSAStartup (versionRequested, & wsaData))
       return -1;

    if (LOBYTE (wsaData.wVersion) != 1||
        HIBYTE (wsaData.wVersion) != 1)
    {
       WSACleanup ();

       return -1;
    }

    timeBeginPeriod (1);

    return 0;
}

void
enet_deinitialize (void)
{
    timeEndPeriod (1);

    WSACleanup ();
}

enet_uint32
enet_time_get (void)
{
    return (enet_uint32) timeGetTime () - timeBase;
}

void
enet_time_set (enet_uint32 newTimeBase)
{
    timeBase = (enet_uint32) timeGetTime () - newTimeBase;
}

int
enet_address_set_host (ENetAddress * address, const char * name)
{
    struct hostent * hostEntry;

    hostEntry = gethostbyname (name);
    if (hostEntry == NULL ||
        hostEntry -> h_addrtype != AF_INET)
    {
        unsigned long host = inet_addr (name);
        if (host == INADDR_NONE)
            return -1;
        address -> host = host;
        return 0;
    }

    address -> host = * (enet_uint32 *) hostEntry -> h_addr_list [0];

    return 0;
}

int
enet_address_get_host_ip (const ENetAddress * address, char * name, size_t nameLength)
{
    char * addr = inet_ntoa (* (struct in_addr *) & address -> host);
    if (addr == NULL)
        return -1;
    strncpy (name, addr, nameLength);
    return 0;
}

int
enet_address_get_host (const ENetAddress * address, char * name, size_t nameLength)
{
    struct in_addr in;
    struct hostent * hostEntry;

    in.s_addr = address -> host;

    hostEntry = gethostbyaddr ((char *) & in, sizeof (struct in_addr), AF_INET);
    if (hostEntry == NULL)
      return enet_address_get_host_ip (address, name, nameLength);

    strncpy (name, hostEntry -> h_name, nameLength);

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
                 sizeof (struct sockaddr_in)) == SOCKET_ERROR ? -1 : 0;
}

int
enet_socket_listen (ENetSocket socket, int backlog)
{
    return listen (socket, backlog < 0 ? SOMAXCONN : backlog) == SOCKET_ERROR ? -1 : 0;
}

ENetSocket
enet_socket_create (ENetSocketType type)
{
    return socket (PF_INET, type == ENET_SOCKET_TYPE_DATAGRAM ? SOCK_DGRAM : SOCK_STREAM, 0);
}

int
enet_socket_set_option (ENetSocket socket, ENetSocketOption option, int value)
{
    int result = SOCKET_ERROR;
    switch (option)
    {
        case ENET_SOCKOPT_NONBLOCK:
        {
            u_long nonBlocking = (u_long) value;
            result = ioctlsocket (socket, FIONBIO, & nonBlocking);
            break;
        }

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
            result = setsockopt (socket, SOL_SOCKET, SO_RCVTIMEO, (char *) & value, sizeof (int));
            break;

        case ENET_SOCKOPT_SNDTIMEO:
            result = setsockopt (socket, SOL_SOCKET, SO_SNDTIMEO, (char *) & value, sizeof (int));
            break;

		case ENET_SOCKOPT_MULTICAST:
		{
			struct ip_mreq mreq;
        	memset(&mreq, 0, sizeof(struct ip_mreq));
        	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        	mreq.imr_multiaddr.s_addr = inet_addr(MDNS_ADDR);
        	result = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq));
		    //setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &value, sizeof(value));
		}
			break;

        default:
            break;
    }
    return result == SOCKET_ERROR ? -1 : 0;
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
		/*{
			int *on = (int *)parm;
			result = setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, on, sizeof(*on));
		}*/
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
    if (result == SOCKET_ERROR && WSAGetLastError () != WSAEWOULDBLOCK)
      return -1;

    return 0;
}

ENetSocket
enet_socket_accept (ENetSocket socket, ENetAddress * address)
{
    SOCKET result;
    struct sockaddr_in sin;
    int sinLength = sizeof (struct sockaddr_in);

    result = accept (socket,
                     address != NULL ? (struct sockaddr *) & sin : NULL,
                     address != NULL ? & sinLength : NULL);

    if (result == INVALID_SOCKET)
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
    return shutdown (socket, (int) how) == SOCKET_ERROR ? -1 : 0;
}

void
enet_socket_destroy (ENetSocket socket)
{
    if (socket != INVALID_SOCKET)
      closesocket (socket);
}

int
enet_socket_send (ENetSocket socket,
                  const ENetAddress * address,
                  const ENetBuffer * buffers,
                  size_t bufferCount)
{
    struct sockaddr_in sin;
    DWORD sentLength;

    if (address != NULL)
    {
        memset (& sin, 0, sizeof (struct sockaddr_in));

        sin.sin_family = AF_INET;
        sin.sin_port = ENET_HOST_TO_NET_16 (address -> port);
        sin.sin_addr.s_addr = address -> host;
    }

    if (WSASendTo (socket,
                   (LPWSABUF) buffers,
                   (DWORD) bufferCount,
                   & sentLength,
                   0,
                   address != NULL ? (struct sockaddr *) & sin : NULL,
                   address != NULL ? sizeof (struct sockaddr_in) : 0,
                   NULL,
                   NULL) == SOCKET_ERROR)
    {
       if (WSAGetLastError () == WSAEWOULDBLOCK)
         return 0;

       return -1;
    }

    return (int) sentLength;
}

int
enet_socket_receive (ENetSocket socket,
                     ENetAddress * address,
                     ENetBuffer * buffers,
                     size_t bufferCount)
{
    INT sinLength = sizeof (struct sockaddr_in);
    DWORD flags = 0,
          recvLength;
    struct sockaddr_in sin;

    if (WSARecvFrom (socket,
                     (LPWSABUF) buffers,
                     (DWORD) bufferCount,
                     & recvLength,
                     & flags,
                     address != NULL ? (struct sockaddr *) & sin : NULL,
                     address != NULL ? & sinLength : NULL,
                     NULL,
                     NULL) == SOCKET_ERROR)
    {
       switch (WSAGetLastError ())
       {
       case WSAEWOULDBLOCK:
       case WSAECONNRESET:
          return 0;
       }

       return -1;
    }

    if (flags & MSG_PARTIAL)
      return -1;

    if (address != NULL)
    {
        address -> host = (enet_uint32) sin.sin_addr.s_addr;
        address -> port = ENET_NET_TO_HOST_16 (sin.sin_port);
    }

    return (int) recvLength;
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
      return -1;

    * condition = ENET_SOCKET_WAIT_NONE;

    if (selectCount == 0)
      return 0;

    if (FD_ISSET (socket, & writeSet))
      * condition |= ENET_SOCKET_WAIT_SEND;

    if (FD_ISSET (socket, & readSet))
      * condition |= ENET_SOCKET_WAIT_RECEIVE;

    return 0;
}

void freeifaddrs( struct ifaddrs *inIFAs )
{
	struct ifaddrs *		p;
	struct ifaddrs *		q;

	// Free each piece of the structure. Set to null after freeing to handle macro-aliased fields.

	for( p = inIFAs; p; p = q )
	{
		q = p->ifa_next;

		if( p->ifa_name )
		{
			free( p->ifa_name );
			p->ifa_name = NULL;
		}
		if( p->ifa_addr )
		{
			free( p->ifa_addr );
			p->ifa_addr = NULL;
		}
		if( p->ifa_netmask )
		{
			free( p->ifa_netmask );
			p->ifa_netmask = NULL;
		}
		if( p->ifa_broadaddr )
		{
			free( p->ifa_broadaddr );
			p->ifa_broadaddr = NULL;
		}
		if( p->ifa_dstaddr )
		{
			free( p->ifa_dstaddr );
			p->ifa_dstaddr = NULL;
		}
		if( p->ifa_data )
		{
			free( p->ifa_data );
			p->ifa_data = NULL;
		}
		free( p );
	}
}

int	getifaddrs_ipv4( struct ifaddrs **outAddrs )
{
	int						err;
	SOCKET					sock;
	DWORD					size;
	DWORD					actualSize;
	INTERFACE_INFO *		buffer;
	INTERFACE_INFO *		tempBuffer;
	INTERFACE_INFO *		ifInfo;
	IP_ADAPTER_INFO *		pAdapterInfo;
	IP_ADAPTER_INFO *		pAdapter;
	ULONG					bufLen;
	int						n;
	int						i;
	struct ifaddrs *		head;
	struct ifaddrs **		next;
	struct ifaddrs *		ifa;

	sock	= INVALID_SOCKET;
	buffer	= NULL;
	head	= NULL;
	next	= &head;
	pAdapterInfo = NULL;

	// Get the interface list. WSAIoctl is called with SIO_GET_INTERFACE_LIST, but since this does not provide a
	// way to determine the size of the interface list beforehand, we have to start with an initial size guess and
	// call WSAIoctl repeatedly with increasing buffer sizes until it succeeds. Limit this to 100 tries for safety.

	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	//err = translate_errno( IsValidSocket( sock ), errno_compat(), kUnknownErr );
	//require_noerr( err, exit );

	n = 0;
	size = 16 * sizeof( INTERFACE_INFO );
	for( ;; )
	{
		tempBuffer = (INTERFACE_INFO *) realloc( buffer, size );
		//require_action( tempBuffer, exit, err = WSAENOBUFS );
		buffer = tempBuffer;

		err = WSAIoctl( sock, SIO_GET_INTERFACE_LIST, NULL, 0, buffer, size, &actualSize, NULL, NULL );
		if( err == 0 )
		{
			break;
		}

		++n;
		require_action( n < 100, exit, err = WSAEADDRNOTAVAIL );

		size += ( 16 * sizeof( INTERFACE_INFO ) );
	}
	//check( actualSize <= size );
	//check( ( actualSize % sizeof( INTERFACE_INFO ) ) == 0 );
	n = (int)( actualSize / sizeof( INTERFACE_INFO ) );

	// Now call GetAdaptersInfo so we can get DHCP information for each interface

	pAdapterInfo	= NULL;
	bufLen			= 0;

	for ( i = 0; i < 100; i++ )
	{
		err = GetAdaptersInfo( pAdapterInfo, &bufLen);

		if ( err != ERROR_BUFFER_OVERFLOW )
		{
			break;
		}

		pAdapterInfo = (IP_ADAPTER_INFO*) realloc( pAdapterInfo, bufLen );

		if ( !pAdapterInfo )
		{
			break;
		}
	}

	// Process the raw interface list and build a linked list of IPv4 interfaces.

	for( i = 0; i < n; ++i )
	{
		unsigned int ifIndex;
		struct sockaddr_in netmask;

		ifInfo = &buffer[ i ];
		if( ifInfo->iiAddress.Address.sa_family != AF_INET )
		{
			continue;
		}

		// <rdar://problem/6220642> iTunes 8: Bonjour doesn't work after upgrading iTunes 8
		// See comment in getifaddrs_ipv6

		ifIndex = 0;
		memset( &netmask, 0, sizeof( netmask ) );
		/*err = AddressToIndexAndMask( ( struct sockaddr* ) &ifInfo->iiAddress.AddressIn, &ifIndex, ( struct sockaddr* ) &netmask );

		if ( err )
		{
			continue;
		}*/

		ifa = (struct ifaddrs *) calloc( 1, sizeof( struct ifaddrs ) );
		require_action( ifa, exit, err = WSAENOBUFS );

		*next = ifa;
		next  = &ifa->ifa_next;

		// Get the name.

		ifa->ifa_name = (char *) malloc( 16 );
		//require_action( ifa->ifa_name, exit, err = WSAENOBUFS );
		sprintf( ifa->ifa_name, "%d", i + 1 );

		// Get interface flags.

		ifa->ifa_flags = (u_int) ifInfo->iiFlags;

		// Get addresses.

		if ( ifInfo->iiAddress.Address.sa_family == AF_INET )
		{
			struct sockaddr_in *		sa4;

			sa4 = &ifInfo->iiAddress.AddressIn;
			ifa->ifa_addr = (struct sockaddr *) calloc( 1, sizeof( *sa4 ) );
			//require_action( ifa->ifa_addr, exit, err = WSAENOBUFS );
			memcpy( ifa->ifa_addr, sa4, sizeof( *sa4 ) );
			//printf("ip : 0x%x\n", (unsigned)sa4->sin_addr.s_addr);
			//printf("ifa_flags : %d\n", (unsigned)ifa->ifa_flags);

			ifa->ifa_netmask = (struct sockaddr*) calloc(1, sizeof( *sa4 ) );
			//require_action( ifa->ifa_netmask, exit, err = WSAENOBUFS );

			// <rdar://problem/4076478> Service won't start on Win2K. The address
			// family field was not being initialized.

			ifa->ifa_netmask->sa_family = AF_INET;
			( ( struct sockaddr_in* ) ifa->ifa_netmask )->sin_addr = netmask.sin_addr;
			ifa->ifa_extra.index = ifIndex;
			//printf("ifa_extra.index : %d\n", (unsigned)ifIndex);
		}
		else
		{
			// Emulate an interface index.

			ifa->ifa_extra.index = (unsigned int)( i + 1 );
		}

		// Now get DHCP configuration information

		for ( pAdapter = pAdapterInfo; pAdapter; pAdapter = pAdapter->Next )
		{
			if ( strcmp( inet_ntoa( ifInfo->iiAddress.AddressIn.sin_addr ), pAdapter->IpAddressList.IpAddress.String ) == 0 )
			{
				ifa->ifa_dhcpEnabled		= pAdapter->DhcpEnabled;
				ifa->ifa_dhcpLeaseExpires	= pAdapter->LeaseExpires;
				//ifa->ifa_womp				= IsWOMPEnabled( pAdapter->AdapterName );

				/*FIXME: here judge the adapter is or not real physics adapter by description having "Virtual" string,
				         but maybe mistake
				*/
				ifa->real_flags = strstr(pAdapter->Description, " Virtual ") ? 0 : 1;
				//printf("adapter description: %s\n", pAdapter->Description);
				//printf("adapter AdapterName: %s\n", pAdapter->AdapterName);
				//printf("adapter Type: %d\n", pAdapter->Type);
				break;
			}
		}
	}

	// Success!

	if( outAddrs )
	{
		*outAddrs = head;
		head = NULL;
	}
	err = 0;

exit:

	if ( pAdapterInfo )
	{
		free( pAdapterInfo );
	}
	if( head )
	{
		freeifaddrs( head );
	}
	if( buffer )
	{
		free( buffer );
	}
	if( sock != INVALID_SOCKET )
	{
		closesocket( sock );
	}
	return( err );
}

int
enet_socket_get_ipv4(int *count, unsigned *iplist)
{
	int             err            = 0;
	int bufSize = count ? *count : 0;
	int counts = 0;

	struct ifaddrs * ips = 0;
	struct ifaddrs * ip;

	if (count == 0 && iplist == 0) err = -1;
	if (bufSize != 0 && iplist == 0) err = -1;
    if (err == 0)
    {
		getifaddrs_ipv4(&ips);
	    ip = ips;
		while(ip)
		{
		    if(ip->ifa_addr->sa_family == AF_INET &&
			    (ip->ifa_flags & IFF_LOOPBACK) != IFF_LOOPBACK  &&
				(ip->ifa_flags & IFF_UP) == IFF_UP //)
				&&
				//(ip->ifa_flags & IFF_MULTICAST) == IFF_MULTICAST &&
				ip->real_flags)
			{
				unsigned int ad = ((struct sockaddr_in *)ip->ifa_addr)->sin_addr.s_addr;
				//printf("the real ip: 0x%x\n", (unsigned)((struct sockaddr_in *)ip->ifa_addr)->sin_addr.s_addr);
				//printf("the real ip: %d.%d.%d.%d\n", (ad & 0xff), ((ad >> 8) & 0xff), ((ad >> 16) & 0xff), (ad >> 24));
				if(counts < bufSize)
				{
					iplist[counts] = ((struct sockaddr_in *)ip->ifa_addr)->sin_addr.s_addr;
				}
				counts++;
			}
			ip = ip->ifa_next;

		}
		if(ips) freeifaddrs(ips);
    }
    if(count) *count = counts;
	if((counts > bufSize && bufSize) || counts == 0) err = -1;

	return err;
}

#endif

