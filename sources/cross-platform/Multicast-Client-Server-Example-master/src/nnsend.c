#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUFSIZE     1024
#define MAXADDRSTR  16
#define LOOPMAX 10
int main(int argc, char *argv[]) {
    int nRet, i;
    int nIP_TTL = 2;
    BOOL  bFlag;
    DWORD dFlag;
    DWORD cbRet;
    int iLen = MAXADDRSTR;
    char strDestMulti[MAXADDRSTR] = "234.5.6.7";
    SOCKADDR_IN stSrcAddr, stDestAddr;
    SOCKET hSock, hNewSock;
    u_short nDestPort = 3456;
    WSABUF stWSABuf;
    char achInBuf [BUFSIZE];
    char achOutBuf[] = "Message number:           ";
    WSADATA stWSAData;

    /* init WinSock */
    WSAStartup(0x0202, &stWSAData);

    /* convert address string to value */
    stDestAddr.sin_family = AF_INET; 
    nRet = WSAStringToAddress (
      strDestMulti,    			/* address string */
      AF_INET,         			/* address family */
      NULL,            			/* protocol info structure */
      (LPSOCKADDR)&stDestAddr,/* socket address string */
      &iLen);          			/* length of socket structure */

    /* get a socket */
    hSock = WSASocket(AF_INET, SOCK_DGRAM, 0,
    (LPWSAPROTOCOL_INFO)NULL, 0, 
    WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF);

    /* allow reuse of the local port number */
    bFlag = TRUE;
    nRet = setsockopt(hSock, SOL_SOCKET, SO_REUSEADDR, 
      (char *)&bFlag, sizeof (bFlag));

    /* name the socket */
    stSrcAddr.sin_family = PF_INET;
    stSrcAddr.sin_port = htons (nDestPort);
    stSrcAddr.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.1.129");//INADDR_ANY;
    nRet = bind (hSock, 
	(struct sockaddr FAR *)&stSrcAddr, 
	sizeof(struct sockaddr));

    /* byte swap host to network order */
    stDestAddr.sin_family = PF_INET;
    nRet = WSAHtons(
		hSock,			/* socket */
		nDestPort,		/* host order value */
		&(stDestAddr.sin_port)); /* network order value */

    /* set the IP TTL */ 
    nRet = WSAIoctl (hSock,   		/* socket */
      SIO_MULTICAST_SCOPE,    		/* IP Time-To-Live */
      &nIP_TTL,               		/* input */
      sizeof (nIP_TTL),        		/* size */
      NULL,                    		/* output */
      0,                       		/* size */
      &cbRet,                  		/* bytes returned */
      NULL,                    		/* overlapped */
      NULL);                   		/* completion routine */

    /* disable loopback */
    bFlag = FALSE;
    nRet = WSAIoctl (hSock,    		/* socket */
      SIO_MULTIPOINT_LOOPBACK,		/* turn loopBack off */
      &bFlag,				/* input */
      sizeof (bFlag),      		/* size */
      NULL,                  		/* output */
      0,		        	/* size */
      &cbRet,            		/* bytes returned */
      NULL,         	 		/* overlapped */
      NULL);                     	/* completion routine */

    /* join the multicast group */
    hNewSock = WSAJoinLeaf (hSock,	/* socket */
      (PSOCKADDR)&stDestAddr, 		/* multicast address */
      sizeof (stDestAddr),    		/* length of addr struct */	
      NULL,                   		/* caller data buffer */
      NULL,                   		/* callee data buffer */
      NULL,                   		/* socket QOS setting */
      NULL,                   		/* socket group QOS */
      JL_BOTH);               		/* do both: send and recv */

    /* send and receive loop */
    for (i=0;i<LOOPMAX;i++) {
	 Sleep (1000);
      static iCounter = 1;
      
      /* send data */
      stWSABuf.buf = achOutBuf;
      stWSABuf.len = lstrlen(achOutBuf);
      cbRet = 0;
      itoa(iCounter++, &achOutBuf[16], 10);
      nRet = WSASendTo (hSock,   	/* socket */
		  &stWSABuf,    	/* output buffer structure */
		  1,            	/* buffer count */
		  &cbRet,       	/* number of bytes sent */
		  0,	        	/* flags */
		  (struct sockaddr*)&stDestAddr,/* destination address */
		  sizeof(struct sockaddr), /* size of addr structure */
		  NULL,	        	/* overlapped structure */
		  NULL);        	/* overlapped callback */

		if (nRet == SOCKET_ERROR) {
		  printf("WSASendTo() failed, Err: %d\n",
 		  WSAGetLastError());
		}

		stWSABuf.buf = achInBuf;
		stWSABuf.len = BUFSIZE;
		cbRet = 0;
		iLen = sizeof (stSrcAddr);
		dFlag = 0;
    		nRet = WSARecvFrom (hSock, /* socket */
		  &stWSABuf,	      /* input buffer structure */
		  1,                       /* buffer count */
		  &cbRet,                  /* number of bytes recv'd */
		  &dFlag,                  /* flags */
        	  (struct sockaddr *)&stSrcAddr,/* source address */
		  &iLen,                   /* size of addr structure */
		  NULL,                    /* overlapped structure */
		  NULL);                   /* overlapped callback */

		if (nRet == SOCKET_ERROR) {
			printf("WSARecvFrom() failed, Err:%d\n",
  			WSAGetLastError());
		} else {
			/* convert address value to string */
			u_short nPort = 0;
			char achAddr[BUFSIZE];
			iLen = BUFSIZE;
			nRet = WSAAddressToString(
				(struct sockaddr *)&stSrcAddr,/* source address */
				sizeof(stSrcAddr),  /* size of addr struct */
				NULL,               /* protocol info */
				achAddr,            /* address string */
				&iLen);             /* addr string buf len */
			
		        /* convert from network to host byte order */
			nRet = WSANtohs(hSock, /* socket */
				stSrcAddr.sin_port, /* host order value */
				&nPort);            /* network order value */
				printf ("Message from %s, port %d: %s\r\n",
			   achAddr[0] ? achAddr : "??", nPort, achInBuf);
			
		        
	    }
	} /* end for(;;) */

  /* tell WinSock we're leaving */
  WSACleanup();

  return 0;
} /* end main() */