#ifndef ENET_MULTICAST_H_INCLUDED
#define ENET_MULTICAST_H_INCLUDED

/*for multicast, add function,
 * so you can search key word "multicast", then found where is changed
 */

ENET_API int        enet_socket_get_ipv4(int *count, unsigned *iplist);
ENET_API int        enet_socket_set_option_ex (ENetSocket, ENetSocketOption, void *);

#endif // ENET-MULTICAST_H_INCLUDED
