#include "RamExecTest.h"

typedef void Loopfunc(UINT32* addr);

void PrintHex(CK_BYTE_PTR sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
	}
	hal_printf("\n");
}

void InfiniteWhileLoop(UINT32* addr){
	while (1){
		hal_printf("ha ha... running from address at %p...\n\n", addr);
	}
}

RamExecTest::RamExecTest( )
{
	memset(largeArray,0,512);
};

BOOL RamExecTest::Execute( )
{
	void *code=(void*)&InfiniteWhileLoop;
	memcpy(largeArray,code,512);

	Loopfunc *ramfunc=(Loopfunc*)(void*)((UINT32)largeArray| 0x01);
	ramfunc((UINT32*)ramfunc);

	//Should never reach here if runs from RAM, but call the code
	//so that linker cleanup does not eliminate the function
	InfiniteWhileLoop((UINT32*)code);

	return TRUE;
} //Execute

void ApplicationEntryPoint()
{
    BOOL result;

    RamExecTest test;

    do
    {
    	if(!test.Execute())
    		hal_printf("Error in RamExec Test.");
    	else
    		hal_printf("RamExec Test is Super Success! ");

    } while(FALSE); // run only once!

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }

}
