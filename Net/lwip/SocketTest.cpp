#include "SocketTest.h"

//set this as true for tcp test, set it to false for UDP
#define TCP_TEST FALSE

void PrintHex(char* sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
	}
	hal_printf("\n");
}



//const ip_addr_t myIP;

SocketTest::SocketTest(BOOL _server )
{
	//ping_init(&myIp)
	servertest=_server;

};

BOOL SocketTest::Init( ){
	if(servertest){
		server_fd=ServerInit(TCP_TEST);
		if(server_fd < 0) return FALSE;
	}else {
		client_fd=ClientInit(TCP_TEST);
		if(client_fd < 0) return FALSE;
	}
	return TRUE;
}

BOOL SocketTest::Execute( )
{
	if(!servertest) {
		if (client_fd > -1){
			for(uint32_t i=0; i< 200; i+=2){
				hal_printf("SocketTest:: Sending packet %d....", i);
				if(SendHello(client_fd,i)){
					hal_printf("Success\n\r", i);
					::Events_WaitForEvents( 0, 10 );
					Recv(client_fd, i);
					::Events_WaitForEvents( 0, 10 );
				}else {
					hal_printf("FAILED. This is bad..\n\r", i);
					return 0;
				}
			}
		}
	}else {
		hal_printf("Socket Server currently not supported\n");
		return FALSE;
	}

	return TRUE;
} //Execute

void ApplicationEntryPoint()
{
    BOOL result;

    //initialize a server
    //SocketTest server(TRUE);
    //initialize a client
    SocketTest client(FALSE);

    do
    {
    	if(client.Init()){
			if(!client.Execute())
				hal_printf("Error in Ping Test.");
			else
				hal_printf("Ping Test is Super Success! ");
    	}else {
    		hal_printf("Test failed! Unable to establish socket");
    	}
    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }

}
