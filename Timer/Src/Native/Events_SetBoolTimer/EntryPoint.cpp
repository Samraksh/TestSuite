////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>

#include "Events_SetBoolTimer.h"

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}


HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{
    BOOL result;
		
	Events_SetBoolTimerTest eventsSetBoolTimerTest(0,0);

    do
    {

    	eventsSetBoolTimerTest.Execute(0);
	  //if(halTimerTest.Execute(0))
		//break;
    } while(FALSE); // run only once!

    while(TRUE);
}
