////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\tinyhal.h>
#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\Samraksh\DataStore\Datastore.h>
#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\Samraksh\HAL_util.h>
#include <cstdlib>
#include "testMath.h"


#define TEST_INITIALIZATION 0
#define TESTREADWRITE_MULTIPLE_RAND_RECORDS 1
#define TESTDELETE 2
#define TESTGETRECORDID 3
#define LEVEL_0A 4
#define LEVEL_0C  5
#define LEVEL_1 6

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
	BOOL 	 TestReadWrite_Multiple_Rand_Records();
	LPVOID 	 CreateDataStoreRecords(int);
	BOOL	 DeleteDataStoreRecords();
	LPVOID	 GetAddress(RECORD_ID);
	BOOL	 GetRecordID();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





