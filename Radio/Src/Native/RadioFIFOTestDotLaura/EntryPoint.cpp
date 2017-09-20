////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
#include "RadioFIFOTest.h"


void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}
//
extern "C" void __cxa_pure_virtual() { while (true); }
void* __dso_handle;
void operator delete(void *, unsigned int){
	while (true);
}

HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{
    BOOL result;
    
    do
    {
    	Test_InitializeAndRun();

    	//dsTestObject.Execute(TEST_INITIALIZATION);

    } while(false); // run only once!

    while(true){
    	::Events_WaitForEvents(0, 100);
    }
}
