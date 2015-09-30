////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "DataStoreTest.h"

//---//

extern Data_Store g_dataStoreObject;
extern Time_Driver g_Time_Driver;
//extern HALTimerManager gHalTimerManagerObject;

UINT16 recordCount = 20;

DataStoreTest dataStoreTestObj;

DataStoreTest::DataStoreTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	testMathInstance.prng_init(3);

};

BOOL DataStoreTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	while(true){
		hal_printf("result=%s\n", (result) ? "PASS":"FAIL");
		hal_printf("accuracy=%d\n", accuracy);
		hal_printf("resultParameter1=%s\n", resultParameter1);
		hal_printf("resultParameter2=%s\n", resultParameter2);
		hal_printf("resultParameter3=null\n");
		hal_printf("resultParameter4=null\n");
		hal_printf("resultParameter5=null\n");
		HAL_Time_Sleep_MicroSeconds(1000000);
	}

	return true;
}

LPVOID DataStoreTest::CreateDataStoreRecords(int count)
{
	LPVOID firstGivenPtr = NULL;
	for(UINT16 index = 1; index <= count; index++)
	{
		if(index == 1)
		{
			firstGivenPtr = g_dataStoreObject.createRecord(index,256,0);
		}
		else
		{
			LPVOID nextPtrs = g_dataStoreObject.createRecord(index,256,0);
		}
	}
	return firstGivenPtr;
}


BOOL DataStoreTest::DeleteDataStoreRecords()
{
	RECORD_ID rId;
	for(rId = 0; rId < 10; rId++)
	{
		if(g_dataStoreObject.deleteRecord(rId) == DATASTORE_STATUS_NOT_OK)
			return false;
	}

	return true;
}

LPVOID DataStoreTest::GetAddress(RECORD_ID rId)
{
	LPVOID retVal = NULL;
	if( (retVal = g_dataStoreObject.getAddress(rId)) != NULL )
		return retVal;
	else
		return NULL;
}

BOOL DataStoreTest::GetRecordID()
{
	test_initialization();

	LPVOID givenPtr = g_dataStoreObject.createRecord(1,256,0);

	if(g_dataStoreObject.getRecordID(givenPtr) != 0)
		return true;
	else
		return false;
}

UINT16 DataStoreTest::GenerateRandomNumber(UINT16 upperBound)
{
	INT64 currTime = g_Time_Driver.TimeNow();
	return (currTime % upperBound + 1);
	//return ( (rand() % upperBound) + 1 );	// +1 because rand() can return a zero, which is not a valid record ID
}


void Timer_3_Handler(void *arg)
{
	if(DataStoreTest::testCompleteCounter_3 == 0){
		if(dataStoreTestObj.Read_And_Write()){
			DataStoreTest::testCompleteCounter_3++;
			//dataStoreTestObj.DisplayStats(true, "SUCCESS : Simple read write successful", NULL, 0);
			hal_printf("SUCCESS Timer_3_Handler: Simple read write successful\n");
		}
		else{
			dataStoreTestObj.DisplayStats(false, (char*)"FAILURE Timer_3_Handler: Simple read write not successful", NULL, 0);
		}
	}
}

void Timer_4_Handler(void *arg)
{
	if(DataStoreTest::testCompleteCounter_4 == 0){
		if(dataStoreTestObj.Read_And_Write()){
			DataStoreTest::testCompleteCounter_4++;
			//dataStoreTestObj.DisplayStats(true, "SUCCESS : Simple read write successful", NULL, 0);
			hal_printf("SUCCESS Timer_4_Handler: Simple read write successful\n");
		}
		else{
			dataStoreTestObj.DisplayStats(false, (char*)"FAILURE Timer_4_Handler: Simple read write not successful", NULL, 0);
		}
	}
}

void Timer_5_Handler(void *arg)
{
	if(DataStoreTest::testCompleteCounter_5 == 0){
		if(dataStoreTestObj.Read_And_Write()){
			DataStoreTest::testCompleteCounter_5++;
			//dataStoreTestObj.DisplayStats(true, "SUCCESS : Simple read write successful", NULL, 0);
			hal_printf("SUCCESS Timer_5_Handler: Simple read write successful\n");
		}
		else{
			dataStoreTestObj.DisplayStats(false, (char*)"FAILURE Timer_5_Handler: Simple read write not successful", NULL, 0);
		}
	}
}

void Timer_6_Handler(void *arg)
{
	if(DataStoreTest::testCompleteCounter_6 == 0){
		if(dataStoreTestObj.Read_And_Write()){
			DataStoreTest::testCompleteCounter_6++;
			//dataStoreTestObj.DisplayStats(true, "SUCCESS : Simple read write successful", NULL, 0);
			hal_printf("SUCCESS Timer_6_Handler: Simple read write successful\n");
		}
		else{
			dataStoreTestObj.DisplayStats(false, (char*)"FAILURE Timer_6_Handler: Simple read write not successful", NULL, 0);
		}
	}
}


BOOL DataStoreTest::Read_And_Write()
{
	UINT16 test_limit = 100;
	char write_data[test_limit];
	char read_data[test_limit];

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		write_data[rwIndex] = rwIndex;
	}

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		read_data[rwIndex] = 0;
	}

	// First write data to sequential records
	for(UINT16 index = 1; index <= recordCount; index++)
	{
		LPVOID givenPtr = GetAddress(index);

		if(givenPtr)
		{
			if( g_dataStoreObject.writeRawData(givenPtr, (void*)write_data, 0, test_limit) )
			{
				//DisplayStats(true, "Success: write data to data store", NULL, 0);
				hal_printf("Success: write data to data store\n");
			}
			else
			{
				DisplayStats(false, (char*)"ERROR: Unable to write data to data store", NULL, 0);
				return false;
			}
		}
		else
		{
			DisplayStats(false, (char*)"ERROR: No valid pointer to record in data store", NULL, 0);
			return false;
		}
	}

	// Then read from random records and compare read and write data
	for(UINT16 index = 1; index <= recordCount; index++)
	{
		UINT16 rId = GenerateRandomNumber(recordCount);
		//UINT16 rId = recordCount;
		LPVOID givenPtr = GetAddress(rId);

		if(givenPtr)
		{
			if( g_dataStoreObject.readRawData(givenPtr, (void*)read_data, 0, test_limit) )
			{
				for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
				{
					if(read_data[rwIndex] != write_data[rwIndex])
					{
						DisplayStats(false, (char*)"ERROR: Read data is not same as write data", NULL, 0);
						return false;
					}
				}
			}
			else
			{
				DisplayStats(false, (char*)"ERROR: Unable to read data from data store", NULL, 0);
				return false;
			}

			//DisplayStats(true, "Success : Simple read write successful", NULL, 0);
			hal_printf("Success : Simple read write successful\n");
		}
		else
		{
			DisplayStats(false, (char*)"ERROR: No valid pointer to record in data store", NULL, 0);
			return false;
		}

		// Reset read_data
		for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
		{
			read_data[rwIndex] = 0;
		}
	}

	return true;
}


// This test writes random data and then verifies if the write has been successful
BOOL DataStoreTest::TestReadWrite_Virtual_Records()
{
	UINT32 sleepTimeLimit = 30000;

	test_initialization();

	g_dataStoreObject.EraseAllBlocks();

	// Create sequential records
	LPVOID firstGivenPtr = CreateDataStoreRecords(recordCount);

	UINT32 dtime = GenerateRandomNumber(sleepTimeLimit);
	//UINT32 dtime = 30000;
	if(!VirtTimer_SetTimer(3, 0, dtime, FALSE, FALSE, Timer_3_Handler))
		return FALSE;

	dtime = GenerateRandomNumber(sleepTimeLimit);
	//dtime = 25000;
	if(!VirtTimer_SetTimer(4, 0, dtime, FALSE, FALSE, Timer_4_Handler))
		return FALSE;

	dtime = GenerateRandomNumber(sleepTimeLimit);
	//dtime = 20000;
	if(!VirtTimer_SetTimer(5, 0, dtime, FALSE, FALSE, Timer_5_Handler))
		return FALSE;

	dtime = GenerateRandomNumber(sleepTimeLimit);
	//dtime = 15000;
	if(!VirtTimer_SetTimer(6, 0, dtime, FALSE, FALSE, Timer_6_Handler))
		return FALSE;

	g_dataStoreObject.EraseAllBlocks();

	for (UINT16 j = 3; j < 7; j++)
	{
		VirtTimer_Start( j );
	}
	
	while(true)
	{
		hal_printf("DataStoreTest::testCompleteCounter_3 %u\n", DataStoreTest::testCompleteCounter_3);
		hal_printf("DataStoreTest::testCompleteCounter_4 %u\n", DataStoreTest::testCompleteCounter_4);
		hal_printf("DataStoreTest::testCompleteCounter_5 %u\n", DataStoreTest::testCompleteCounter_5);
		hal_printf("DataStoreTest::testCompleteCounter_6 %u\n", DataStoreTest::testCompleteCounter_6);
		if(DataStoreTest::testCompleteCounter_3 == 1 && DataStoreTest::testCompleteCounter_4 == 1 && DataStoreTest::testCompleteCounter_5 == 1 &&
    			DataStoreTest::testCompleteCounter_6 == 1)
    	{
    		hal_printf("result=PASS\n");
			hal_printf("resultParameter1=DataStore test Level_4A completed successfully\n");
			HAL_Time_Sleep_MicroSeconds(1000000);
    	}
	}

	return true;

}

BOOL DataStoreTest::test_initialization(void)
{
	int eraseDataStore = true;
	if(g_dataStoreObject.init(eraseDataStore) != DS_Success)
	{
		DisplayStats(false, (char*)"ERROR: Could not initialize data store", NULL, 0);
		return false;
	}
	else
	{
		//DisplayStats(true, "SUCCESS: initialize data store", NULL, 0);
		hal_printf("SUCCESS: initialize data store\n");
		return true;
	}
}


BOOL DataStoreTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
		case 0:
			result = test_initialization();
			break;
		case 1:
			result = TestReadWrite_Virtual_Records();
			break;
		case 2:
			result = DeleteDataStoreRecords();
			break;
		case 3:
			result = GetRecordID();
			break;
		default:
			DisplayStats(false, (char*)"ERROR: Not a valid option", NULL, 0);
			break;
	}

	return result;

} //Execute

