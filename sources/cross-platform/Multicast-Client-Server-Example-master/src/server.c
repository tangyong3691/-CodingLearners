/* server.c
 * This sample demonstrates a multicast server that works with either
 * IPv4 or IPv6, depending on the multicast address given.
 *
 * Troubleshoot Windows: Make sure you have the IPv6 stack installed by running
 *     >ipv6 install
 *
 * Usage:
 *     server <Multicast Address> <Port> <packetsize> <defer_ms> [<TTL>>]
 *
 * Examples:
 *     >server 224.0.22.1 9210 6000 1000
 *     >server ff15::1 2001 65000 1
 *
 * Written by tmouse, July 2005
 * http://cboard.cprogramming.com/showthread.php?t=67469
 *
 * Modified to run multi-platform by Christian Beier <dontmind@freeshell.org>.
 */




#include <stdio.h>
#include <stdlib.h>
#ifdef UNIX
#include <unistd.h> /* for usleep() */
#endif
#include "msock.h"



static void DieWithError(char* errorMessage)
{
  fprintf(stderr, "%s\n", errorMessage);
  exit(EXIT_FAILURE);
}

/*DATA for send, 10 hex of data*/
/*data is index9 table, */
#define DATA0 "\x01\x00\x01\x12\x36\x02\x00\x21\x36\x02"
#define DATA1 "\x00\x00\x09\x30\x00\x00\x00\x00\x00\x00"
#define DATA2 "\xdd\xcc\xbb\xaa\x4a\x20\x8f\xd0\x55\x03"
#define DATA3 "\x2b\x00\x2b\x00\x00\x00\x01\x00\x00\x00"

/*udp data check format: code+data, code 2byte, data bytes defined by index table*/
//#define DATA4 "\x07\x00\x63\x81\x01\x00\x33\x03\x05\x00"
#define DATA4 "\x07\x00\x02\x01\x01\x00\x33\x03\x05\x00"
#define DATA5 "\x37\x08\x00\xfd\xff\xff\xff\x02\x00\xff"
#define DATA6 "\xff\xff\xff\x04\x00\x63\x81\x09\x00\x17"
#define DATA7 "\x00\x00\x00\x00\x00"


int main(int argc, char *argv[])
{
  SOCKET sock;
  struct addrinfo *multicastAddr;
  char*     localIP;
  char*     multicastIP;            /* Arg: IP Multicast address */
  char*     multicastPort;          /* Arg: Server port */
  char*     sendString = 0;             /* Arg: String to multicast */
  int       sendStringLen;          /* Length of string to multicast */
  int       multicastTTL;           /* Arg: TTL of multicast packets */
  int       defer_ms;               /* miliseconds to defer in between sending */
  int  sendcountmax;
  char  senddata[512];
  int i;
  //fprintf(stdout, "check argument: %d\n", argc);
  if ( argc < 2 || argc > 8 )
    {
      fprintf(stdout, "Usage: %s <local ip> [<Multicast ip>] [<Port>] [<packetsize>] [repeat] [<defer_ms>] [<TTL>]\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  localIP = argv[1];
  multicastIP   = (argc >= 3 )? argv[2] : "225.0.111.2";             /* First arg:   multicast IP address */
  multicastPort = (argc >= 4 )? argv[3] : "5566";             /* Second arg:  multicast port */
  sendStringLen = (argc >= 5 )? atoi(argv[4]): 10;   
  sendcountmax= (argc >= 6 )? atoi(argv[5]): 1; 
  defer_ms = (argc >= 7 )? atoi(argv[6]): 1000;
  multicastTTL  = (argc == 8 ?         /*  arg:  If supplied, use command-line */
		   atoi(argv[7]) : 1); /* specified TTL, else use default TTL of 1 */
   
   fprintf(stdout, "multicast: %s:%s, local ip:%s\n", multicastIP, multicastPort, localIP);
  /* just fill this with some byte */
  if(sendStringLen > 0 && sendStringLen < 512){
    sendString = senddata;//(char*)calloc(sendStringLen, sizeof(char));
    for(i = 0; i<sendStringLen; ++i)
       sendString[i]= 's';
    sendString[i]= 0;
  }	else {
   memcpy(senddata,  DATA0, 10);
   memcpy(&senddata[10],  DATA1, 10);
   memcpy(&senddata[20],  DATA2, 10);
   memcpy(&senddata[30],  DATA3, 10);
   memcpy(&senddata[40],  DATA4, 10);
   memcpy(&senddata[50],  DATA5, 10);
   memcpy(&senddata[60],  DATA6, 10);
   memcpy(&senddata[70],  DATA7, 5);
    sendStringLen = 75;
  }

  



  sock = mcast_send_socket(localIP, multicastIP, multicastPort, multicastTTL, &multicastAddr);
  if(sock == -1 )
      DieWithError("mcast_send_socket() failed");
  
  fprintf(stdout,"starting send\n");
  unsigned int nr=0;
  unsigned short hexval07 = 0;
  for (;;) /* Run forever */
    {
      //int*  p_nr = (int*)sendString;
      //*p_nr = htonl(nr);
      if(!sendString)
	  {
		  hexval07++;
		  *(short*)&senddata[42]=hexval07;
	  }
      if ( sendto(sock, senddata, sendStringLen, 0,
		  multicastAddr->ai_addr, multicastAddr->ai_addrlen) != sendStringLen )
	  DieWithError("sendto() sent a different number of bytes than expected");
      if(  sendcountmax > 0 || (nr % 60) == 0 )  //3600
        fprintf(stderr, "packet %d sent\n", nr);
      nr++;
	  if(nr == (long long)sendcountmax) break;
#ifdef UNIX
      usleep(defer_ms*1000); 
#else 
      Sleep (defer_ms);
#endif 

      
    }

  /* NOT REACHED */
  return 0;
}

