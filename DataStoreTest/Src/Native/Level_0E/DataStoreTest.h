////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <..\DeviceCode\Include\tinyhal.h>
#include <..\DeviceCode\Include\Samraksh\DataStore\Datastore.h>
#include <..\DeviceCode\Include\Samraksh\HAL_util.h>
#include <cstdlib>
#include "testMath.h"


#define TEST_INITIALIZATION 0
#define TESTREADWRITE_SEQ_CHAR 1
#define TESTDELETE 2
#define TESTGETADDRESS 3
#define TESTGETRECORDID 4
#define LEVEL_0A 5
#define LEVEL_0C  6
#define LEVEL_1 7

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
	BOOL 	 TestReadWrite_Sequential_CHAR();
	LPVOID 	 CreateDataStoreRecords(int);
	BOOL	 DeleteDataStoreRecords();
	BOOL	 GetAddress();
	BOOL	 GetRecordID();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





