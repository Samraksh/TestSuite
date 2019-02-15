#ifndef _SOCKETTEST_H_
#define _SOCKETTEST_H_

#include <tinyhal.h>
//#include <LWIP_Sockets.h>
#include <lwip/sockets.h>

#define SENDER_PORT_NUM 6000
#define SENDER_IP_ADDR "192.168.5.100"

#define SERVER_PORT_NUM 6001
#define SERVER_IP_ADDRESS "192.168.5.200"


int ServerInit(BOOL _tcpTest);
int ClientInit(BOOL _tcpTest);
BOOL SendHello(int socket_fd, uint32_t iter);
BOOL Recv(int socket_fd, uint32_t iter);

int client_fd, server_fd;
char data_buffer[256];

class SocketTest
{
	//char largeArray[512];
	BOOL servertest;
public:
	SocketTest (BOOL server);
	BOOL Init();
    BOOL   Execute();

};




#endif //_SOCKETTEST_H_
