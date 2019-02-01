#include "SocketTest.h"

//#include <ping.h>

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

	if(servertest){
		server_fd=ServerInit();
	}else {
		client_fd=ClientInit();
	}
};

BOOL SocketTest::Execute( )
{
	if(!servertest) {
		if (client_fd > -1){
			for(int i=0; i< 10; i++){
				hal_printf("SocketTest:: Sending packet %d....", i);
				if(SendHello(client_fd,i)){
					hal_printf("SUCCESS\n\r", i);
					//::Events_WaitForEvents( 0, 1000 );
				}else {
					hal_printf("FAILED. This is bad..\n\r", i);
					return 0;
				}

			}
		}
	}else {
		if(server_fd > -1){

		}
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
    	if(!client.Execute())
    		hal_printf("Error in Ping Test.");
    	else
    		hal_printf("Ping Test is Super Success! ");

    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }

}
