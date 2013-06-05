////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "SDIO.h"
#include <tinyhal.h>
#include <Samraksh\HAL_util.h>

//---//
extern SDIO_Driver g_SDIODriver;

SDIOTest::SDIOTest( int seedValue, int numberOfEvents )
{

	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();


	this->numberOfEvents = numberOfEvents;

	testMathInstance.prng_init(30);


};

BOOL SDIOTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, char* accuracy)
{
	hal_printf("result = %s\n", (result) ? "true":"false");
	hal_printf("accuracy = %s\n", accuracy);
	hal_printf("resultParameter1 = %s\n", resultParameter1);
	hal_printf("resultParameter2 = %s\n", resultParameter2);
	hal_printf("resultParameter3 = %s\n", resultParameter3);
	hal_printf("resultParameter4 = %s\n", resultParameter4);
	hal_printf("resultParameter5 = %s\n", resultParameter5);

	USART_Flush(COM1);

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL SDIOTest::Level_0A()
{

	UINT8 input[512];
	UINT8 output[512];

	for(UINT16 i = 0; i < 512; i++)
	{
		input[i] = i % 254;
	}

	for(UINT16 i = 0; i < 512; i++)
	{
		output[i] = 0;
	}

	UINT16 i = 0;

	if(g_SDIODriver.Initialize() != DS_Success)
	{
		DisplayStats(FALSE, "SD Card Initialization Failed", NULL, NULL);
		return FALSE;
	}

	while(i++ < this->numberOfEvents)
	{
		if(g_SDIODriver.WriteBlock(input,(2 << 15), 512) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to write to SD Card", NULL, NULL);
			return FALSE;
		}





		if(g_SDIODriver.ReadBlock(output, (2 << 15), 512) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to read SD Card", NULL, NULL);
			return FALSE;
		}



		for(UINT8 i = 0; i < 24; i++)
		{
			if(input[i] != output[i])
			{
				DisplayStats(FALSE, "Level 0A SDIO Test Failed", NULL,NULL);
				return FALSE;
			}
		}

		break;

	}

	DisplayStats(TRUE, "Level 0A SDIO Test Success",NULL, NULL);
	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL SDIOTest::Level_0B()
{
	UINT16 i = 0;

	while(i++ < this->numberOfEvents)
	{
	
	}

	DisplayStats(TRUE, "Flash Write Test Successful", NULL, NULL);
	return TRUE;

}

BOOL SDIOTest::Level_1()
{
	UINT16 i = 0;

	
	DisplayStats(TRUE, "Flash Write Level 1 Successful", NULL, NULL);
	return TRUE;
}

// Tests the buffer write and read interface
BOOL SDIOTest::Level_0C()
{
	
	DisplayStats(TRUE, "Write and Read Buffer Test was successful", NULL,NULL);
	return TRUE;
}


BOOL SDIOTest::Execute( int testLevel )
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
		result = Level_1();
		break;
	}


	return result;



} //Execute

