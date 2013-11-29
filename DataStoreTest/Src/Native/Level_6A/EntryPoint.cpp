////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\tinyhal.h>
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

    	dsTestObject.Execute(TESTREADWRITE_PERSISTENCE_MULTIPLE_SEQ_RECORDS);

    } while(false); // run only once!

    while(true);
}
