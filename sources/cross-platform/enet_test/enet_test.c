/* test program by call enet library
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
*/

#ifdef _WIN32
#define _WIN32_WINNT 0x0501
#endif

#include "enet/enet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#define LOGTRACE(fmt, args...)  do{ printf ("%s(%d)%s: " fmt, __FILE__, __LINE__,__func__, ## args); }while(0)

#define MULTICAST_ENABLE
//#define BROADCAST_ENABLE

#ifdef TEST_PLATFORM_WINDOWS

#ifdef _WIN32

//#define MAX_ADAPTER_ADDRESS_LENGTH 8
//#define MAX_ADAPTER_DESCRIPTION_LENGTH 128
//#define MAX_ADAPTER_NAME_LENGTH 256

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

/*typedef struct {
  char String[16];
} IP_ADDRESS_STRING, *PIP_ADDRESS_STRING, IP_MASK_STRING, *PIP_MASK_STRING;*/

/*typedef struct _IP_ADDR_STRING {
  struct _IP_ADDR_STRING* Next;
  IP_ADDRESS_STRING IpAddress;
  IP_MASK_STRING IpMask;
  DWORD Context;
} IP_ADDR_STRING, *PIP_ADDR_STRING;*/

/*typedef struct _IP_ADAPTER_INFO {
  struct _IP_ADAPTER_INFO* Next;
  DWORD ComboIndex;
  char AdapterName[MAX_ADAPTER_NAME_LENGTH+4];
  char Description[MAX_ADAPTER_DESCRIPTION_LENGTH+4];
  UINT AddressLength;
  BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
  DWORD Index;
  UINT Type;
  UINT DhcpEnabled;
  PIP_ADDR_STRING CurrentIpAddress;
  IP_ADDR_STRING IpAddressList;
  IP_ADDR_STRING GatewayList;
  IP_ADDR_STRING DhcpServer;
  BOOL HaveWins;
  IP_ADDR_STRING PrimaryWinsServer;
  IP_ADDR_STRING SecondaryWinsServer;
  time_t LeaseObtained;
  time_t LeaseExpires;
} IP_ADAPTER_INFO, *PIP_ADAPTER_INFO;*/

#define require_action(X, LABEL, ACTION)   \
         do { \
		        if ( !(X) ) \
				{ \
				    {ACTION;} \
					goto LABEL; \
				} \
			} while( 0 )
		

		
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
				
				/*FIXME: here judge the adapter is or not real physics adapter by description having "PCI" string,
				         but maybe mistake
				*/
				ifa->real_flags = strstr(pAdapter->Description, "PCI") ? 1 : 0;
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

#endif

#endif

int ismemberof(int value, int* group, int nmemb)
{
	int ii;
	for(ii = 0; ii < nmemb; ii++)
	{
		if(group[ii] == value)
			return 1;
	}
	return 0;
}

static unsigned long getip(const char *const name)
{    
        unsigned long ip = 0; 
        struct addrinfo hints;
        struct addrinfo *addrs = NULL;
		struct addrinfo *inf;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
		hints.ai_protocol = IPPROTO_IP;
		//hints.ai_flags = AI_ADDRCONFIG;
     
        if (getaddrinfo(name, NULL, &hints, &addrs) == 0)
                {    
                ip = ((struct sockaddr_in*) addrs->ai_addr)->sin_addr.s_addr;
					for(inf = addrs; inf != 0; inf = inf->ai_next)
					{
						//printf("ip list: 0x%x\n", (unsigned)(((struct sockaddr_in*) inf->ai_addr)->sin_addr.s_addr));
					}
                }

        if (addrs)
                {
                freeaddrinfo(addrs);
                }    

        return(ip);
}    


void test_udp()
{
#define MULTICAST_IP "224.30.30.200"
#define MULTICAST_PORT 1234
	//ENetSocket sock;
	ENetSocket sock[8];
	int sendflg[8];
	ENetSocket maxsock = 0;
	//ENetSocket sockr;
	ENetAddress addr;
	ENetAddress dest;
	ENetAddress recaddr;
	ENetBuffer buf;
	ENetBuffer recbuf;
	enet_uint32 waitflg;
	enet_uint32 baseflg;
	int ret;
	char *msg = "test msg,say hello\n";
	char rec[1500];
    int send_max = 10;
    int rec_count = 0;
		
	char hostname[256] = {0,};
	unsigned int* iplist = 0;
	int ipcnt = 0;
	int ii = 0;
	unsigned int multi_ip;
	int multi_port;
	struct sockaddr_in multicast_addr[2];
	int op;
	char opc;
	ENetSocketSet readset;
	ENetSocketSet writeset;
	
	memset(sendflg, 0, sizeof(sendflg));
	
	if(enet_initialize() != 0)
	{
	       LOGTRACE("enet init err\n");	
		return;
	}
	
	multi_ip = inet_addr(MULTICAST_IP);
	multi_port = MULTICAST_PORT;
	
	//gethostname(hostname, 256);
	//printf("hostname:%s\n", hostname);
	//addr.host = getip(hostname);
	//printf("host ip:0x%x\n", (unsigned) addr.host);
#ifdef TEST_PLATFORM_WINDOWS
	{
		struct ifaddrs * ips = 0;
		struct ifaddrs * ip;
		getifaddrs_ipv4(&ips);
	    ip = ips;
		while(ip)
		{
		    if(ip->ifa_addr->sa_family == AF_INET &&
			    (ip->ifa_flags & IFF_LOOPBACK) != IFF_LOOPBACK  &&
				(ip->ifa_flags & IFF_UP) == IFF_UP &&
				//(ip->ifa_flags & IFF_MULTICAST) == IFF_MULTICAST &&
				ip->real_flags)
			{
				unsigned int ad = ((struct sockaddr_in *)ip->ifa_addr)->sin_addr.s_addr;
				//printf("the real ip: 0x%x\n", (unsigned)((struct sockaddr_in *)ip->ifa_addr)->sin_addr.s_addr);
				printf("the real ip: %d.%d.%d.%d\n", (ad & 0xff), ((ad >> 8) & 0xff), ((ad >> 16) & 0xff), (ad >> 24));
			}
			ip = ip->ifa_next;
				
		}
		if(ips) freeifaddrs(ips);
	}
#endif

	//unsigned int* iplist = 0;
	//int ipcnt = 0;
	
	ret = enet_socket_get_ipv4(&ipcnt, 0);
	if(!ipcnt)
	{
		printf("err: no find valid local ip\n");
		enet_deinitialize();
		return;
	}
	
	if(!ret)
	{
		printf("ip totals = %d\n", ipcnt);		
		iplist = calloc(ipcnt, sizeof(unsigned int));
		ret = enet_socket_get_ipv4(&ipcnt, iplist);
		for(ii = 0; ii < ipcnt; ii++)
		{
			unsigned int ad = iplist[ii];
			printf("the real ip: %d.%d.%d.%d\n", (ad & 0xff), ((ad >> 8) & 0xff), ((ad >> 16) & 0xff), (ad >> 24));
		}
	}
	
	//enet_deinitialize();
	//return ;
	addr.host = INADDR_ANY;
	addr.port = 2001;
	
	dest.port = 2001;
	dest.host = 192 + (168<<8) + (1<<16) + (155<<24);
#ifdef BROADCAST_ENABLE
	dest.host = 255 + (255<<8) + (255<<16) + (255<<24);
#endif
#ifdef MULTICAST_ENABLE
	//dest.host = 224 + (30<<8) + (30<<16) + (200<<24);
	//dest.host = 224 + (0<<8) + (0<<16) + (251<<24);
	dest.host = multi_ip;
	dest.port = multi_port;
	addr.port = multi_port;
#endif
        printf("udp test, ip:%d.%d.%d.%d, port:%d\n", 
			dest.host%256, 
			(dest.host>>8)%256,  
			(dest.host>>16)%256,  
			(dest.host>>24)%256,
			dest.port);
	printf("local bind port:%d\n", addr.port);
	buf.dataLength = strlen(msg);
	buf.data = msg;

	recbuf.dataLength = 1500;
	recbuf.data = rec;

	if(sizeof(sock) / sizeof(sock[0]) < ipcnt)
	{
		ipcnt = sizeof(sock) / sizeof(sock[0]);
		printf("warning: ip too big , can't open all\n");
	}
     
    for(ii = 0; ii < ipcnt; ii++)
    {	
	sock[ii] = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
	if(sock[ii] == ENET_SOCKET_NULL)
	{
		LOGTRACE("socket create err\n");
		//enet_deinitialize();
		//return;
		continue;
	}
	
	enet_socket_set_option(sock[ii], ENET_SOCKOPT_REUSEADDR, 1);

	if(enet_socket_bind(sock[ii], &addr) == -1)
	{
		LOGTRACE("socket bind err\n");
		enet_socket_destroy(sock[ii]);
		sock[ii] = ENET_SOCKET_NULL;
		//enet_deinitialize();
		//return;
		continue;
	}
	
	maxsock = maxsock > sock[ii] ? maxsock : sock[ii];
	
	}
    
	/*for(ii = 0; ii < ipcnt; ii++)
		if(sock[ii] != ENET_SOCKET_NULL)
			enet_socket_set_option(sock[ii], ENET_SOCKOPT_REUSEADDR, 1);*/
#ifdef BROADCAST_ENABLE
	enet_socket_set_option(sock[0], ENET_SOCKOPT_BROADCAST, 1);
#endif
#ifdef MULTICAST_ENABLE
    memset(multicast_addr, 0, sizeof(multicast_addr));
	multicast_addr[1].sin_addr.s_addr = multi_ip;
	for(ii = 0; ii < ipcnt; ii++)
	{
		if(sock[ii] != ENET_SOCKET_NULL)
		{
			multicast_addr[0].sin_addr.s_addr = iplist[ii];
			enet_socket_set_option_ex(sock[ii], ENET_SOCKOPT_MULTICAST, multicast_addr);
			op = 1;
			enet_socket_set_option_ex(sock[ii], ENET_SOCKOPT_PKTINFO, &op);
			enet_socket_set_option_ex(sock[ii], ENET_SOCKOPT_MULTICAST_IF, multicast_addr);
			opc = 255;
			enet_socket_set_option_ex(sock[ii], ENET_SOCKOPT_MULTICAST_TTL, &opc);
		}
	}
#endif
	enet_time_set(0);
	baseflg = ENET_SOCKET_WAIT_RECEIVE;
        do{

        //LOGTRACE("loop\n");
	if(enet_time_get() >= 2000)
	{
		//LOGTRACE("set wait send\n");
		baseflg |= ENET_SOCKET_WAIT_SEND;
		for(ii = 0; ii < ipcnt; ii++)
			if(sock[ii] != ENET_SOCKET_NULL)
				sendflg[ii] = ENET_SOCKET_WAIT_SEND;
		enet_time_set(0);
	}
	do{
        	waitflg = baseflg;
		//ret = enet_socket_wait(sock, &waitflg, 100);
		ENET_SOCKETSET_EMPTY(readset);
		ENET_SOCKETSET_EMPTY(writeset);
		for(ii = 0; ii < ipcnt; ii++)
		{
			if(sock[ii] != ENET_SOCKET_NULL)
			{
				ENET_SOCKETSET_ADD(readset, sock[ii]);
				if(sendflg[ii]) ENET_SOCKETSET_ADD(writeset, sock[ii]);
			}		
		}
		ret = enet_socketset_select(maxsock, &readset, &writeset, 100);
		
	}while((ret == -1)); // || (waitflg == ENET_SOCKET_WAIT_NONE));
        //LOGTRACE("waited\n");
	for(ii = 0; ii < ipcnt; ii++)
	{
		if(sock[ii] != ENET_SOCKET_NULL)
		{
			if(ENET_SOCKETSET_CHECK(readset, sock[ii]))
			{
				ret = enet_socket_receive(sock[ii], &recaddr, &recbuf,1);
				if(ret > 0)
				{
				rec[ret] = 0;
				if( (!ismemberof(recaddr.host, iplist, ipcnt)) || recaddr.port != multi_port)
				{
				printf("local ip:%d.%d.%d.%d receive data:%s\n", 
				     iplist[ii]%256,
					 (iplist[ii]>>8)%256,
					 (iplist[ii]>>16)%256,
					 (iplist[ii]>>24)%256,
					rec);

				printf("from %d.%d.%d.%d : %d\n", 
				       recaddr.host%256, 
					   (recaddr.host>>8)%256, 
					   (recaddr.host>>16)%256, 
					   (recaddr.host>>24)%256, 
					   recaddr.port);
				printf("cnt:%d\n", ++rec_count); 
				}
				}
				else
				{
				LOGTRACE("socket ii=%d receive ret = %d\n", ii, ret);
				}
			}
			if(ENET_SOCKETSET_CHECK(writeset, sock[ii]))
			{
				sendflg[ii] = 0;
				ret = enet_socket_send(sock[ii], &dest, &buf, 1);
				if(ret <=0 )
				{
					LOGTRACE("socket send ret = %d\n", ret);
				}	
				else
				{
					LOGTRACE("socket send success, ret = %d\n", ret);
					--send_max;
				}
			}
		}
	}
	/*if(waitflg & ENET_SOCKET_WAIT_SEND)
	{
		baseflg &= ~ENET_SOCKET_WAIT_SEND; 
	        ret = enet_socket_send(sock, &dest, &buf, 1);
		if(ret <=0 )
		{
			LOGTRACE("socket send ret = %d\n", ret);
		}
		else
		{
			LOGTRACE("socket send success, ret = %d\n", ret);
			--send_max;
		}
	}

	if(waitflg & ENET_SOCKET_WAIT_RECEIVE)
	{
			LOGTRACE("socket receive waited\n");
			ret = enet_socket_receive(sock, &recaddr, &recbuf,1);
			if(ret > 0)
			{
				rec[ret] = 0;
				printf("receive data:%s\n", rec);
				printf("from 0x%x : %d\n", recaddr.host, recaddr.port);
				printf("cnt:%d\n", ++rec_count); 
			}
			else
			{
				LOGTRACE("socket receive ret = %d\n", ret);
			}
	}*/
	}while(send_max);
	
	if(iplist) free(iplist);
	for(ii = 0; ii < ipcnt; ii++)
	{
		if(sock[ii] != ENET_SOCKET_NULL)
			enet_socket_destroy(sock[ii]);
	}
	enet_deinitialize();
	
	LOGTRACE("enet test fin\n");	
}


int main(int argc, char* argv[])
{
	test_udp();
}
