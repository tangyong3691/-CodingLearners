#ifndef _PING_H_
#define _PING_H_

int ping_test(char *url);

typedef enum
{
	PING_BIND_ERROR = -10,
	PING_ADDRESS_NULL_ERROR,
	PING_GET_ADDRESS_ERROR,
	PING_SEND_ERROR,
	PING_RECV_ERROR = -1
}PING_ERROR;

#endif

