////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <..\DeviceCode\Include\tinyhal.h>
#include <..\DeviceCode\Include\Samraksh\DataStore\Datastore.h>
#include <..\DeviceCode\Include\Samraksh\HAL_util.h>
#include <cstdlib>
#include "testMath.h"


#define TEST_INITIALIZATION 0
#define TEST_FILLFLASH 1
#define TESTDELETE 2
#define TESTGETRECORDID 3
#define LEVEL_0A 4
#define LEVEL_0C  5
#define LEVEL_1 6

#define MAX_NUM_TABLE_ENTRIES       (256)

class DataStoreTest
{

	int numberOfEvents;

	testMath testMathInstance;

public:

//--//
    DataStoreTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL     Level_0C();

	BOOL 	 Level_1();

	BOOL 	 test_initialization();
	BOOL 	 StartReading();
	BOOL 	 Test_FillFlash();
	LPVOID 	 CreateDataStoreRecords(int, int);
	BOOL	 DeleteDataStoreRecords();
	LPVOID	 GetAddress(RECORD_ID);
	BOOL	 GetRecordID();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





