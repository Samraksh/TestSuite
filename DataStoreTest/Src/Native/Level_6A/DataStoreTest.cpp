////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "DataStoreTest.h"

//---//

// PA0 - 15 --- 0 to 15
// PB0 - 15 --- 16 to 31
// PC0 - 15 --- 32 to 37
//J14_PIN4 is PB14 --- which is 30
#define J12_PIN4 30

extern Data_Store g_dataStoreObject;


DataStoreTest::DataStoreTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	testMathInstance.prng_init(3);

};

BOOL DataStoreTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return true;
}

// This test only checks if the manufacture id can be read
BOOL DataStoreTest::Level_0A()
{

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
		return false;
}

BOOL DataStoreTest::GetRecordID()
{
	test_initialization();

	LPVOID givenPtr = g_dataStoreObject.createRecord(1,256,0);

	if(g_dataStoreObject.getRecordID(givenPtr) != NULL)
		return true;
	else
		return false;
}


// This test writes random data and then verifies if the write has been successful
/* This is how the test works:
 * 1. Create "recordCount" records.
 * 2. Each record has "test_limit" number of data. (1 2 3 4....test_limit)
 * 3. First write all records. While writing, also read from the records. After a successful read, reset the record with all 1's.
 * 4. Then while reading, break at a random point (to mimic a crash). The variable "crashTest" indicates that a crash has occurred.
 * 5. Then call the app again. (TestReadWrite_Persistence_Multiple_Seq_Records)
 * 6. Now build the recordIDs. In order to find out where the app crashed, this line is used -- if(read_data[test_limit-1] == (test_limit - 1)).
 * 	  This line will not be true for any of the records before the crash as they have been set with 1's.
 * 7. Now the point of crash is stored here -- recIndexCrashTest = tempIndex;
 * 8. Now the read begins from where it left off.
 * 9. Since we are now in the recursive path, as soon as we reach the end of read, the end of recursion is indicated by -- reversePath = 1;
 * 10. On the way up, since the variable "reversePath" is set, the app now does not enter the loop and the program ends tamely. */
BOOL DataStoreTest::TestReadWrite_Persistence_Multiple_Seq_Records()
{
	UINT16 test_limit = 100;
	char write_data[test_limit];
	char read_data[test_limit];
	char reset_data[test_limit];
	uint32 recordID_array[256] = {};
	UINT16 recordCount = 20;
	UINT16 recIndexCrashTest = 0;
	static int crashTest = 0, reversePath = 0;
	CPU_GPIO_EnableOutputPin((GPIO_PIN) J12_PIN4, false);

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		write_data[rwIndex] = rwIndex;
	}

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		read_data[rwIndex] = 0;
		reset_data[rwIndex] = 1;
	}

	test_initialization();

	//g_dataStoreObject.DeleteAll();

	//myVector table1;

	LPVOID firstGivenPtr = CreateDataStoreRecords(recordCount);

	g_dataStoreObject.init();
	g_dataStoreObject.EraseAllBlocks();
	//// DATASTORE_STATUS status = g_dataStoreObject.initDataStore( "NOR", &defaultProperty );
	////g_dataStoreObject.scanFlashDevice();

	//myVector table2;

	// First write data to successive records
	if(!crashTest)
	{
		for(UINT16 recIndex = 1; recIndex <= recordCount; recIndex++)
		{
			LPVOID givenPtr = GetAddress(recIndex);

			if(givenPtr)
			{
				if( g_dataStoreObject.writeRawData(givenPtr, (void*)write_data, 0, test_limit) )
				{
					DisplayStats(true, "Success: write data to data store", NULL, 0);
				}
				else
				{
					DisplayStats(false, "ERROR: Unable to write data to data store", NULL, 0);
					return false;
				}
			}
			else
			{
				DisplayStats(false, "ERROR: No valid pointer to record in data store", NULL, 0);
			}
		}
	}


	/*UINT16 persistenceIndex = 0;
	while(g_dataStoreObject.addressTable.table[persistenceIndex].recordID != 0)
	{
		recordID_array[persistenceIndex] = g_dataStoreObject.addressTable.table[persistenceIndex].recordID;
		persistenceIndex++;
	}*/

	// If the crashTest variable is set, then do persistence scan.
	if(crashTest)
	{
		g_dataStoreObject.getRecordIDAfterPersistence(recordID_array, 256, 0);
		//*(recordID_array++)
		//(*recordID_array)++)
		/*while(*recordID_array != 0)
			recordID_array++;
		recIndexCrashTest = *recordID_array;*/
		for(int tempIndex = 1; tempIndex <= recordCount; tempIndex++)
		{
			LPVOID givenPtr = GetAddress(tempIndex);
			g_dataStoreObject.readRawData(givenPtr, (void*)read_data, 0, test_limit);

			if(read_data[test_limit-1] == (test_limit - 1))
			{
				recIndexCrashTest = tempIndex;
				break;
			}
		}

	}
	else
	{
		recIndexCrashTest = 1;
	}


	// Then read from the records and compare read and write data
	for(; recIndexCrashTest <= recordCount; recIndexCrashTest++)
	{
		if(recIndexCrashTest == recordCount)
			reversePath = 1;

		if( recIndexCrashTest == 10 && (!crashTest) ){
			CPU_GPIO_SetPinState((GPIO_PIN) J12_PIN4, true);
			crashTest = 1;
			TestReadWrite_Persistence_Multiple_Seq_Records();
		}

		if(!reversePath)
		{
			LPVOID givenPtr = GetAddress(recIndexCrashTest);

			if(givenPtr)
			{
				if( g_dataStoreObject.readRawData(givenPtr, (void*)read_data, 0, test_limit) )
				{
					g_dataStoreObject.writeRawData(givenPtr, (void*)reset_data, 0, test_limit);
					for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
					{
						if(read_data[rwIndex] != write_data[rwIndex])
						{
							DisplayStats(false, "ERROR: Read data is not same as write data", NULL, 0);
							return false;
						}
					}
				}
				else
				{
					DisplayStats(false, "ERROR: Unable to read data from data store", NULL, 0);
					return false;
				}

				DisplayStats(true, "Success : Simple read write successful", NULL, 0);
			}
			else
			{
				DisplayStats(false, "ERROR: No valid pointer to record in data store", NULL, 0);
			}

			// Reset read_data
			for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
			{
				read_data[rwIndex] = 0;
			}
		}
	}

	DisplayStats(true, "SUCCESS : Simple read write successful", NULL, 0);
	g_dataStoreObject.EraseAllBlocks();
	return true;

}

BOOL DataStoreTest::test_initialization(void)
{
	if(g_dataStoreObject.init() != DS_Success)
	{
		DisplayStats(false, "ERROR: Could not initialize data store", NULL, 0);
		return false;
	}
	else
	{
		DisplayStats(true, "SUCCESS: initialize data store", NULL, 0);
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

	switch(testLevel)
	{
		case 0:
			result = test_initialization();
			break;
		case 1:
			result = TestReadWrite_Persistence_Multiple_Seq_Records();
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

