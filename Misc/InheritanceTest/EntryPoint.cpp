////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
#include "InheritanceTestObjectt.h"


void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}


HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

//extern void (*__preinit_array_start []) (void) __attribute__((weak));
//extern void (*__preinit_array_end []) (void) __attribute__((weak));
//extern void (*__init_array_start []) (void) __attribute__((weak));
//extern void (*__init_array_end []) (void) __attribute__((weak));


void ApplicationEntryPoint()
{

//	  // This is basically libc_init_array -- handles global constructors
//	  unsigned int count;
//	  unsigned int i;
//
//	  count = __preinit_array_end - __preinit_array_start;
//	  for (i = 0; i < count; i++)
//	    __preinit_array_start[i] ();
//
//	  count = __init_array_end - __init_array_start;
//	  for (i = 0; i < count; i++)
//	    __init_array_start[i] ();


    BOOL result;
    
    do
    {
    	InheritanceTest::InheritanceTest_InitializeGlobalPointers();
    	InheritanceTest::InheritanceTestTest_InitializeAndRun();

    	//dsTestObject.Execute(TEST_INITIALIZATION);

    } while(false); // run only once!

    while(true){
    	::Events_WaitForEvents(0, 100);
    }
}
