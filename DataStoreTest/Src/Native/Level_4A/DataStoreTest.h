////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <..\DeviceCode\Include\tinyhal.h>
#include <..\DeviceCode\Include\Samraksh\DataStore\Datastore.h>
#include <..\DeviceCode\Include\Samraksh\HAL_util.h>
#include <..\DeviceCode\Include\Samraksh\HALTimer.h>
#include <cstdlib>
#include "testMath.h"


#define TEST_INITIALIZATION 0
#define TESTREADWRITE_VIRTUAL_RECORDS 1
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
	DataStoreTest();

	DataStoreTest ( int seedValue, int numberOfEvents );


    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL     Level_0C();

	BOOL 	 Level_1();

	BOOL 	 test_initialization();
	BOOL 	 TestReadWrite_Virtual_Records();
	LPVOID 	 CreateDataStoreRecords(int);
	BOOL	 DeleteDataStoreRecords();
	LPVOID	 GetAddress(RECORD_ID);
	BOOL	 GetRecordID();
	BOOL	 Read_And_Write();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

	UINT16	 GenerateRandomNumber(UINT16);
	/*void	 Timer_1_Handler(LPVOID);
	void	 Timer_2_Handler(LPVOID);
	void	 Timer_3_Handler(LPVOID);
	void	 Timer_4_Handler(LPVOID);*/

};





