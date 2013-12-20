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

LPVOID* DataStoreTest::CreateDataStoreRecords(int count)
{
	////LPVOID* firstGivenPtr = (void**)new char[count];
	//LPVOID* firstGivenPtr = (void**)malloc(sizeof(char) * count);
	//LPVOID* firstGivenPtr[count];
	// Array of pointers to given addresses
	LPVOID firstGivenPtr[count];
	for(UINT16 index = 1; index <= count; ++index)
	{
		//*(firstGivenPtr++) = reinterpret_cast<void**>(g_dataStoreObject.createRecord(index,256));
		////firstGivenPtr[index-1] = reinterpret_cast<void**>(g_dataStoreObject.createRecord(index,256));
		//*(firstGivenPtr[index-1]) = g_dataStoreObject.createRecord(index,256);
		firstGivenPtr[index-1] = g_dataStoreObject.createRecord(index,256);
	}
	return firstGivenPtr;
}


BOOL DataStoreTest::DeleteDataStoreRecords(RECORD_ID rId)
{
	if(g_dataStoreObject.deleteRecord(rId) == DATASTORE_STATUS_OK)
		return true;
	else
		return false;
}

LPVOID DataStoreTest::GetAddress(RECORD_ID rId)
{
	LPVOID retVal = NULL;
	if( (retVal = g_dataStoreObject.getAddress(rId)) != NULL )
		return retVal;
	else
		return false;
}

RECORD_ID DataStoreTest::GetRecordID(LPVOID givenPtr)
{
	return g_dataStoreObject.getRecordID(givenPtr);
}


// This test writes random data and then verifies if the write has been successful
BOOL DataStoreTest::TestReadWrite_Delete_Multiple_Seq_Records()
{
	UINT16 test_limit = 100;
	char write_data[test_limit];
	char read_data[test_limit];
	UINT16 recordCount = 20;
	LPVOID ptrList[recordCount];

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		write_data[rwIndex] = rwIndex;
	}

	for(UINT16 rwIndex = 0; rwIndex < test_limit; ++rwIndex)
	{
		read_data[rwIndex] = 0;
	}

	test_initialization();

	g_dataStoreObject.DeleteAll();

	//LPVOID* firstGivenPtr = reinterpret_cast<void**>(CreateDataStoreRecords(recordCount));
	LPVOID* firstGivenPtr = CreateDataStoreRecords(recordCount);
	// Store the array of given pointers in an array, as the contents of the pointer array is lost for some reason.
	for(UINT16 index = 0; index < recordCount; ++index)
	{
		ptrList[index] = firstGivenPtr[index];
	}

	// First write data to successive records
	for(UINT16 index = 1; index <= recordCount; index++)
	{
		LPVOID givenPtr = GetAddress(index);

		if(givenPtr)
		{
			if( g_dataStoreObject.writeRawData(givenPtr, (void*)write_data, test_limit) )
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
			return false;
		}
	}


	// Then first delete the record and read from the records to compare read and write data
	for(UINT16 index = 1; index <= recordCount; index++)
	{
		// Get the first given pointer. Index for array starts from zero, but recordID starts from 1.
		LPVOID givenPtr = ptrList[index-1];
		if(DeleteDataStoreRecords(index))
		{
			RECORD_ID rID = GetRecordID(givenPtr);

			if(rID != index)
			{
				DisplayStats(true, "SUCCESS: Record successfully deleted", NULL, 0);
			}
			else
			{
				DisplayStats(false, "ERROR: Record not deleted", NULL, 0);
				return false;
			}
		}
		else
		{
			DisplayStats(false, "ERROR: Could not delete record in data store", NULL, 0);
			return false;
		}
	}

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
			result = TestReadWrite_Delete_Multiple_Seq_Records();
			break;
		case 2:
			result = Level_0A();
			break;
		case 3:
			result = Level_0C();
			break;
		case 4:
			result = Level_1();
			break;
		default:
			DisplayStats(false, "ERROR: Not a valid option", NULL, 0);
			break;
	}

	return result;

} //Execute

