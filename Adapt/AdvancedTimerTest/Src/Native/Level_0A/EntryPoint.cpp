////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include "D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\tinyhal.h"

#include "AdvancedTimerTest.h"

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
		
    AdvancedTimerTest advancedTimerTest(0,0);

    do
    {

    	if(!advancedTimerTest.Execute(0))
    		hal_printf("Error");
	  //if(halTimerTest.Execute(0))
		//break;
    } while(FALSE); // run only once!

    while(TRUE);
}
