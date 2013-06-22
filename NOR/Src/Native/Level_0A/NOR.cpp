////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "NOR.h"
#include <Samraksh\HAL_util.h>

//---//

extern P30BF65NOR_Driver gNORDriver;

NORTest::NORTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	gNORDriver.Initialize();

	this->numberOfEvents = numberOfEvents;

	testMathInstance.prng_init(30);

};

BOOL NORTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL NORTest::Level_0A()
{

	UINT16 i = 0;
	int j;
	bool testPassed = false;

	while(i++ < this->numberOfEvents)
	{
		gNORDriver.ReadID();

		if(gNORDriver.GetManufactureId() == MANUFACTURE_ID)
		{
			testPassed = true;
		}

		for(j = 0; j < 100000; j++){}
	}

	if (testPassed == true)
		DisplayStats(TRUE, "Reading Manufacture Successful", NULL, 0);
	else
		DisplayStats(FALSE, "Reading Manufacture ID Failed", NULL, 0);

	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL NORTest::Level_0B()
{
	UINT16 i = 0;

	while(i++ < this->numberOfEvents)
	{
		gNORDriver.ReadID();

		if(gNORDriver.GetManufactureId() == MANUFACTURE_ID)
		{

			if(gNORDriver.ReadHalfWord(0x0) != 0xffff)
			{
				if(gNORDriver.EraseBlock(0x0) != DS_Success)
				{
					DisplayStats(FALSE, "Erasing the flash failed",  NULL, NULL);
					return FALSE;
				}
			}

			for(UINT32 i = 0; i < 10; i++)
			{
				gNORDriver.WriteHalfWord(i * 0x2, i);

				if(gNORDriver.ReadHalfWord(i * 0x2) != i)
				{
					DisplayStats(FALSE, "Writing to flash failed", NULL, NULL);
					return FALSE;
				}
			}
		}
		else
		{
			DisplayStats(FALSE, "Reading Manufacture id failed", NULL, NULL);
			return FALSE;
		}
	}

	DisplayStats(TRUE, "Flash Write Test Successful", NULL, NULL);
	return TRUE;

}

BOOL NORTest::Level_1()
{
	UINT16 i = 0;

	UINT16 inputdata[20];
	UINT16 outputdata[20];

	for(UINT32 i = 0; i < 100000; i++);

	while(i++ < this->numberOfEvents)
	{
		UINT32 address = (testMathInstance.pareto_prng() % 126) * 64 * 1024;

		for(UINT16 j = 0; j < 20; j++)
		{
			outputdata[j] = 0;
		}

		for(UINT16 j = 0; j < 20; j++)
		{
			inputdata[j] = (UINT16) (testMathInstance.pareto_prng() % (1 << 15));
		}

		hal_printf("Writing at address %d\n", address);

		gNORDriver.ReadID();

		if(gNORDriver.GetManufactureId() == MANUFACTURE_ID)
		{
			if(gNORDriver.ReadHalfWord(address) != 0xffff)
			{
				if(gNORDriver.EraseBlock(address) != DS_Success)
				{
					DisplayStats(FALSE, "Erasing the flash failed",  NULL, NULL);
					return FALSE;
				}

			}

			if(gNORDriver.WriteBuffer(inputdata, address, 20) == DS_Success)
			{
				gNORDriver.ReadBuffer(outputdata, address, 20);
			}
			else
			{
				DisplayStats(FALSE, "Write Buffer failed", NULL, NULL);
				return FALSE;
			}
		}

	}

	DisplayStats(TRUE, "Flash Write Level 1 Successful", NULL, NULL);
	return TRUE;
}

// Tests the buffer write and read interface
BOOL NORTest::Level_0C()
{
	UINT16 i = 0;

	UINT16 inputdata[20];
	UINT16 outputdata[20];

	for(UINT16 j = 0; j < 20; j++)
	{
		outputdata[j] = 0;
	}

	for(UINT16 j = 0; j < 20; j++)
	{
		inputdata[j] = (UINT16) (testMathInstance.pareto_prng() % (1 << 15));
	}

	while(i++ < this->numberOfEvents)
	{
		gNORDriver.ReadID();

		if(gNORDriver.GetManufactureId() == MANUFACTURE_ID)
		{
			if(gNORDriver.ReadHalfWord(0x0) != 0xffff)
			{
				if(gNORDriver.EraseBlock(0x0) != DS_Success)
				{
					DisplayStats(FALSE, "Erasing the flash failed",  NULL, NULL);
					return FALSE;
				}
			}

			if(gNORDriver.WriteBuffer(inputdata, 0x0, 20) == DS_Success)
			{
				gNORDriver.ReadBuffer(outputdata, 0x0, 20);
			}
			else
			{
				DisplayStats(FALSE, "Write Buffer failed", NULL, NULL);
				return FALSE;
			}

		}
		else
		{
			DisplayStats(FALSE, "Reading Manufacture id failed", NULL, NULL);
			return FALSE;
		}

		for(UINT16 i = 0; i < 20; i++)
		{
			if(inputdata[i] != outputdata[i])
			{
				DisplayStats(FALSE, "Read data is not same as written data", NULL,NULL);
				return FALSE;
			}
		}
	}

	DisplayStats(TRUE, "Write and Read Buffer Test was successful", NULL,NULL);
	return TRUE;
}


// Tests the buffer write and read interface for various address values
BOOL NORTest::Level_0D()
{
	UINT16 i = 0;

	UINT16 inputdata[20];
	UINT16 outputdata[20];

	for(UINT16 j = 0; j < 20; j++)
	{
		outputdata[j] = 0;
	}

	for(UINT16 j = 0; j < 20; j++)
	{
		inputdata[j] = (UINT16) (testMathInstance.pareto_prng() % (1 << 15));
	}

	while(i++ < this->numberOfEvents)
	{
		gNORDriver.ReadID();

		if(gNORDriver.GetManufactureId() == MANUFACTURE_ID)
		{
			if(gNORDriver.ReadHalfWord(0x8000) != 0xffff)
			{
				if(gNORDriver.EraseBlock(0x8000) != DS_Success)
				{
					DisplayStats(FALSE, "Erasing the flash failed",  NULL, NULL);
					return FALSE;
				}
			}

			if(gNORDriver.WriteBuffer(inputdata, 0x8000, 20) == DS_Success)
			{
				gNORDriver.ReadBuffer(outputdata, 0x8000, 20);
			}
			else
			{
				DisplayStats(FALSE, "Write Buffer failed", NULL, NULL);
				return FALSE;
			}

		}
		else
		{
			DisplayStats(FALSE, "Reading Manufacture id failed", NULL, NULL);
			return FALSE;
		}

		for(UINT16 i = 0; i < 20; i++)
		{
			if(inputdata[i] != outputdata[i])
			{
				DisplayStats(FALSE, "Read data is not same as written data", NULL,NULL);
				return FALSE;
			}
		}
	}

	DisplayStats(TRUE, "Write and Read Buffer Test was successful", NULL,NULL);
	return TRUE;
}


BOOL NORTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	case 2:
		result = Level_0C();
		break;
	case 3:
		result = Level_0D();
		break;
	case 4:
		result = Level_1();
		break;
	}


	return result;



} //Execute

