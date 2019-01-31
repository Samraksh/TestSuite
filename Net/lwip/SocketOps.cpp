#include "SocketTest.h"


int ServerInit(void)
{

	int socket_fd,accept_fd;
	socklen_t addr_size;
	int sent_data;
	char data_buffer[80]; struct sockaddr_in sa,ra,isa;

	// Creates an TCP socket (SOCK_STREAM) with Internet Protocol Family (PF_INET).
	// Protocol family and Address family related. For example PF_INET Protocol Family and AF_INET family are coupled.
	server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if ( server_fd < 0 )
	{
		hal_printf("SocketTest: ServerInit: socket call failed\n");
		return FALSE;
	}

	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(SENDER_IP_ADDR);
	sa.sin_port = htons(SENDER_PORT_NUM);


	// Bind the TCP socket to the port SENDER_PORT_NUM and to the current
	// machines IP address (Its defined by SENDER_IP_ADDR).
	// Once bind is successful for UDP sockets application can operate
	// on the socket descriptor for sending or receiving data.
	if (bind(server_fd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		hal_printf("SocketTest: ServerInit: Bind to Port Number %d ,IP address %s failed\n",SENDER_PORT_NUM,SENDER_IP_ADDR);
		close(server_fd);
		return FALSE;
	}

	listen(server_fd,5);
	addr_size = sizeof(isa);
	accept_fd = accept(server_fd, (struct sockaddr*)&isa,&addr_size);
	if(accept_fd < 0)
	{
		hal_printf("SocketTest: ServerInit: accept failed\n");
		close(server_fd);
		return FALSE;
	}

	return server_fd;
}


int ClientInit(void)
{
	struct sockaddr_in sa,ra;

	// Creates an TCP socket (SOCK_STREAM) with Internet Protocol Family (PF_INET).
	// Protocol family and Address family related. For example PF_INET Protocol Family and AF_INET family are coupled.
	client_fd=-1;
	client_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if ( client_fd < 0 )
	{
		hal_printf("SocketTest: ClientInit: socket call failed\n");
		return FALSE;
	}

	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(SENDER_IP_ADDR);
	sa.sin_port = htons(SENDER_PORT_NUM);


	/* Bind the TCP socket to the port SENDER_PORT_NUM and to the current
	* machines IP address (Its defined by SENDER_IP_ADDR).
	* Once bind is successful for UDP sockets application can operate
	* on the socket descriptor for sending or receiving data.
	*/
	if (bind(client_fd, (struct sockaddr *)&sa, sizeof(struct sockaddr)) == -1)
	{
		hal_printf("SocketTest: ClientInit:Bind to Port Number %d ,IP address %s failed\n",SENDER_PORT_NUM,SENDER_IP_ADDR);
		close(client_fd);
		return FALSE;
	}

	// Receiver connects to server ip-address.
	memset(&ra, 0, sizeof(struct sockaddr_in));
	ra.sin_family = AF_INET;
	ra.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
	ra.sin_port = htons(SERVER_PORT_NUM);


	if(connect(client_fd,(struct sockaddr*)&ra,sizeof(struct sockaddr)) < 0)
	{
		hal_printf("SocketTest: Client connect failed \n");
		close(client_fd);
		return FALSE;
	}
	return TRUE;
}

BOOL Recv(int socket_fd, int iter){
	int recv_data = recv(socket_fd,data_buffer,sizeof(data_buffer),0);
	if(recv_data < 0)
	{
		hal_printf("SocketTest: Recv %d failed \n", iter);
		return FALSE;
	}
	data_buffer[recv_data] = '\0';
	hal_printf("received data: %s\n",data_buffer);

	return TRUE;
}


BOOL SendHello(int socket_fd, int iter){
	memcpy(data_buffer,"Hello World\n",12);
	int sent_data = send(socket_fd, data_buffer,sizeof("Hello World\n"),0);

	if(sent_data < 0 )
	{
		hal_printf("SocketTest: Socket sending %d  failed\n", iter);
		return FALSE;
	}
	return TRUE;
}

BOOL Close(int socket_fd){
	close(socket_fd);
}
