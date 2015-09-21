////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
#include "DataStoreTest.h"


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
    

    do
    {
    	DataStoreTest dsTestObject(0,100);

    	dsTestObject.Execute(TESTREADWRITE_VIRTUAL_RECORDS);

    } while(false); // run only once!

    while(true){
    	::Events_WaitForEvents(0, 100);
    	if(DataStoreTest::testCompleteCounter_3 == 1 && DataStoreTest::testCompleteCounter_4 == 1 && DataStoreTest::testCompleteCounter_5 == 1 &&
    			DataStoreTest::testCompleteCounter_6 == 1)
    	{
    		hal_printf("result=PASS\n");
			hal_printf("resultParameter1=DataStore test Level_4A completed successfully\n");
			HAL_Time_Sleep_MicroSeconds(1000000);
    	}
    }
}
