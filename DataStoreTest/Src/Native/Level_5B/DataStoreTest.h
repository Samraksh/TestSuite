////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <..\DeviceCode\Include\tinyhal.h>
#include <..\DeviceCode\Include\Samraksh\DataStore\Datastore.h>
#include <..\DeviceCode\Include\Samraksh\HAL_util.h>
#include <cstdlib>
#include "testMath.h"


#define TEST_INITIALIZATION 0
#define TESTREADWRITE_DELETE_MULTIPLE_SEQ_RECORDS 1
#define LEVEL_0A 2
#define LEVEL_0C  3
#define LEVEL_1 4

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
	BOOL 	 TestReadWrite_Delete_Multiple_Seq_Records();
	LPVOID*	 CreateDataStoreRecords(int);
	BOOL	 DeleteDataStoreRecords(RECORD_ID);
	LPVOID	 GetAddress(RECORD_ID);
	RECORD_ID	 GetRecordID(LPVOID);

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





