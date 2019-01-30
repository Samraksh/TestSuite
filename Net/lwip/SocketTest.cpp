#include "SocketTest.h"
#include <ping.h>

void PrintHex(CK_BYTE_PTR sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
	}
	hal_printf("\n");
}


const ip_addr_t myIP;

SocketTest::SocketTest( )
{
	ping_init(&myIp)
};

BOOL SocketTest::Execute( )
{

	for(int i=0; i< 10; i++){
		hal_printf("Sending ping %d....", i);
		ping_send_now()
		::Events_WaitForEvents( 0, 100 );
	}

	return TRUE;
} //Execute

void ApplicationEntryPoint()
{
    BOOL result;

    SocketTest test;

    do
    {
    	if(!test.Execute())
    		hal_printf("Error in Ping Test.");
    	else
    		hal_printf("Ping Test is Super Success! ");

    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }

}
