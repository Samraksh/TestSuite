////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "DataStoreTest.h"

//---//

extern Data_Store g_dataStoreObject;

//J11 Pin 3 - PA0 (#0)
//J11 Pin 8 - PA8 (#8)
//PA15 - #15
//J12 Pin 1 - PB8 (#24)
//J12 Pin 5 - PB15 (#31)
#define LED1_FOR_ERASE 8 	//(J11_Pin8 -- PA8)
#define LED2_FOR_WRITE 30 	//(J12_Pin4 -- PB14)
#define LED3_FOR_READ 31	//(J12_Pin5 -- PB15)


const double definedFlashSize = (125.0 * 131072.0) + 5000.0;     //Size of the flash is: 125 * 131072 = 16384000 (~16MB). Little more than actual flash size in order to check if exception is thrown.
//const double definedFlashSize = g_dataStoreObject.returnTotalSpace() + 5000;

const int objectSize = 1000;
UINT16 test_limit = objectSize;
UINT16 objectCount = 16384;

BOOL endOfObjectsFlag = false;
UINT16 finalObjectID = 0;



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

// This test only checks if the manufacture id can be read
BOOL DataStoreTest::Level_0A()
{
	return true;
}

LPVOID DataStoreTest::CreateDataStoreRecords(int count, int i)
{
	LPVOID firstGivenPtr = NULL;
	for(UINT16 index = 1+(i-1)*count; index <= i*count; index++)
	{
		firstGivenPtr = g_dataStoreObject.createRecord(index,objectSize,0);
		if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_OUT_OF_FLASH_MEMORY) {
			finalObjectID = index-1;
			endOfObjectsFlag = true;
			break;
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
		return false;
}

BOOL DataStoreTest::GetRecordID()
{
	test_initialization();

	LPVOID givenPtr = g_dataStoreObject.createRecord(1,objectSize,0);

	if(g_dataStoreObject.getRecordID(givenPtr) != NULL)
		return true;
	else
		return false;
}

BOOL DataStoreTest::StartReading()
{
	char write_data[test_limit];
	char read_data[test_limit];

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex) {
		write_data[rwIndex] = rwIndex;
	}

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex) {
		read_data[rwIndex] = 0;
	}

	for(UINT16 index = 1; index <= objectCount; index+=100)
	{
		hal_printf("index=%d\n", index);
		if(index > finalObjectID) {
			break;
		}

		LPVOID givenPtr = GetAddress(index);

		if(givenPtr) {
			UINT32 numBytesRead = g_dataStoreObject.readRawData(givenPtr, (void*)read_data, 0, test_limit);
			if( numBytesRead == test_limit)
			{
				for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
				{
					if(read_data[rwIndex] != write_data[rwIndex]) {
						DisplayStats(false, "ERROR: Read data is not same as write data", NULL, 0);
						return false;
					}
				}
			}
			else {
				DisplayStats(false, "ERROR: Unable to read data from data store", NULL, 0);
				return false;
			}

			//Clear read_data
			for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex) {
				read_data[rwIndex] = 0;
			}
		}
		else {
			DisplayStats(false, "ERROR: No valid pointer to record in data store", NULL, 0);
			return false;
		}
	}
	return true;
}


// This test writes random data and then verifies if the write has been successful
BOOL DataStoreTest::Test_FillFlash()
{
	INT64 startTime = HAL_Time_CurrentTime();
	hal_printf("start time: %lld \n", startTime);
	char write_data[test_limit];

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex) {
		write_data[rwIndex] = rwIndex;
	}

	test_initialization();

	int flashUsedSpace = g_dataStoreObject.returnLogPoint();
	int flashFreeSpace = g_dataStoreObject.returnFreeSpace();
	int totSpace = g_dataStoreObject.returnTotalSpace();

	hal_printf("flashUsedSpace = %d\n", flashUsedSpace);
	hal_printf("flashFreeSpace = %d\n", flashFreeSpace);
	int i = 1;

	/* Until the flash is full, keep writing. After it is full, start reading. */
	while(flashUsedSpace <= definedFlashSize)
	{
		int objectsToBeCreated = MAX_NUM_TABLE_ENTRIES;
		LPVOID firstGivenPtr = CreateDataStoreRecords(objectsToBeCreated, i);

		for(UINT16 index = 1+((i-1)*objectsToBeCreated); index <= i*objectsToBeCreated; index++)
		{
			LPVOID givenPtr = GetAddress(index);

			//If object is valid, write data to it.
			if(givenPtr)
			{
				UINT32 numBytesWritten = g_dataStoreObject.writeRawData(givenPtr, (void*)write_data, 0, test_limit);
				if( numBytesWritten ==  test_limit) {
					continue;
				}
				else {
					if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_OUT_OF_FLASH_MEMORY) {
							goto END_OF_TEST;
					} else {
						DisplayStats(false, "ERROR: Unable to write data to data store", NULL, 0);
						return false;
					}
				}
			}
			else {
				if(endOfObjectsFlag) {
						goto END_OF_TEST;
				}
				else {
					if(g_dataStoreObject.getLastError() == DATASTORE_ERROR_OUT_OF_FLASH_MEMORY) {
							goto END_OF_TEST;
					}
					else {
						DisplayStats(false, "ERROR: No valid pointer to record in data store", NULL, 0);
					}
				}
			}
		}
		i++;
		flashUsedSpace = g_dataStoreObject.returnLogPoint();
		flashFreeSpace = g_dataStoreObject.returnFreeSpace();
		hal_printf("flashUsedSpace = %d\n", flashUsedSpace);
		hal_printf("flashFreeSpace = %d\n", flashFreeSpace);
	}

END_OF_TEST:
	INT64 endTime = HAL_Time_CurrentTime();
	hal_printf("end time: %lld \n", endTime);
	hal_printf("total time taken: %lld \n", endTime - startTime);
	
	return true;

}

BOOL DataStoreTest::test_initialization(void)
{
	int eraseDataStore = true;
	if(g_dataStoreObject.init(eraseDataStore) != DS_Success)
	{
		DisplayStats(false, "ERROR: Could not initialize data store", NULL, 0);
		return false;
	}
	else
	{
		//DisplayStats(true, "SUCCESS: initialize data store", NULL, 0);
		hal_printf("SUCCESS: initialize data store \n");
		return true;
	}

}

BOOL DataStoreTest::Level_1()
{
	return true;
}

// Tests the buffer write and read interface
BOOL DataStoreTest::Level_0C()
{
	return true;
}



BOOL DataStoreTest::Execute( int testLevel )
{
	BOOL result;
	BOOL readResult = false;

	switch(testLevel)
	{
		case 0:
			result = test_initialization();
			break;
		case 1:
			result = Test_FillFlash();
			readResult = StartReading();
			if(readResult) {
				g_dataStoreObject.EraseAllBlocks();
				DisplayStats(true, "SUCCESS: flash fill test succeeded", NULL, 0);
			} else {
				DisplayStats(false, "FAILURE: flash fill test failed", NULL, 0);
				return false;
			}
			break;
		case 2:
			result = DeleteDataStoreRecords();
			break;
		case 3:
			result = GetRecordID();
			break;
		case 4:
			result = Level_0A();
			break;
		case 5:
			result = Level_0C();
			break;
		case 6:
			result = Level_1();
			break;
		default:
			DisplayStats(false, "ERROR: Not a valid option", NULL, 0);
			break;
	}

	return result;

} //Execute

