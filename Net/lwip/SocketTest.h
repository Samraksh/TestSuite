#ifndef _SOCKETTEST_H_
#define _SOCKETTEST_H_

#include <tinyhal.h>
//#include <LWIP_Sockets.h>
#include <lwip/sockets.h>

#define SENDER_PORT_NUM 6000
#define SENDER_IP_ADDR "192.168.5.100"

#define SERVER_PORT_NUM 6001
#define SERVER_IP_ADDRESS "192.168.5.200"


int ServerInit(void);
int ClientInit(void);
BOOL SendHello(int socket_fd, int iter);
BOOL Recv(int socket_fd, int iter);

int client_fd, server_fd;
char data_buffer[256];

class SocketTest
{
	//char largeArray[512];
	BOOL servertest;
public:
	SocketTest (BOOL server);
    BOOL   Execute();

};




#endif //_SOCKETTEST_H_
