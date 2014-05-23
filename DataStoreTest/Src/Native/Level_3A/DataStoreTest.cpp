////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "DataStoreTest.h"

//---//

extern Data_Store g_dataStoreObject;


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
BOOL DataStoreTest::TestReadWrite_Multiple_Seq_Records()
{
	UINT16 test_limit = 100;
	char write_data[test_limit];
	char read_data[test_limit];
	UINT16 recordCount = 20;

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		write_data[rwIndex] = rwIndex;
	}

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		read_data[rwIndex] = 0;
	}

	test_initialization();

	g_dataStoreObject.EraseAllBlocks();

	LPVOID firstGivenPtr = CreateDataStoreRecords(recordCount);

	// First write data to successive records
	for(UINT16 index = 1; index <= recordCount; index++)
	{
		LPVOID givenPtr = GetAddress(index);

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

	// Then read from the records and compare read and write data
	for(UINT16 index = 1; index <= recordCount; index++)
	{
		LPVOID givenPtr = GetAddress(index);

		if(givenPtr)
		{
			if( g_dataStoreObject.readRawData(givenPtr, (void*)read_data, 0, test_limit) )
			{
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
			result = TestReadWrite_Multiple_Seq_Records();
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

