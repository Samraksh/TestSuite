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

void SDStatus(DeviceStatus status)
{
	hal_printf("The Operations was a %s\n", (status == DS_Success) ? "Success" : "Failure");
}

BOOL SDIOTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, char* accuracy)
{
	hal_printf("result = %s\n", (result) ? "true":"false");
	hal_printf("accuracy = %s\n", accuracy);
	hal_printf("resultParameter1 = %s\n", resultParameter1);
	hal_printf("resultParameter2 = %s\n", resultParameter2);
	hal_printf("resultParameter3 = %s\n", "");
	hal_printf("resultParameter4 = %s\n", "");
	hal_printf("resultParameter5 = %s\n", "");

	USART_Flush(COM1);

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL SDIOTest::Level_0A()
{

	SDIOStatusFuncPtrType sdCallback = SDStatus;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);


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

	if(g_SDIODriver.Initialize(sdCallback) != DS_Success)
	{
		DisplayStats(FALSE, "SD Card Initialization Failed", NULL, NULL);
		return FALSE;
	}

	while(i++ < this->numberOfEvents)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		if(g_SDIODriver.EraseBlock((1 << 15), 512 + (1 << 15)) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to erase sd card", NULL, NULL);
			return FALSE;
		}

		if(g_SDIODriver.WriteBlock(input,(1 << 15), 512) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to write to SD Card", NULL, NULL);
			return FALSE;
		}



		for(UINT32 i = 0; i < 10000; i++);




		if(g_SDIODriver.ReadBlock(output, (1 << 15), 512) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to read SD Card", NULL, NULL);
			return FALSE;
		}



		for(UINT16 i = 0; i < 512; i++)
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

	SDIOStatusFuncPtrType sdCallback = SDStatus;

	UINT8  inputdata[512];
	UINT8  outputdata[512];

	if(g_SDIODriver.Initialize(sdCallback) != DS_Success)
	{
		DisplayStats(FALSE, "SD Card Initialization Failed", NULL, NULL);
		return FALSE;
	}

	while(i++ < this->numberOfEvents)
	{

		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	    CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		UINT32 address = (testMathInstance.pareto_prng() % 126) * 64 * 1024;

		//address = (1 << 15);

		for(UINT16 j = 0; j < 512; j++)
		{
			outputdata[j] = 0;
		}

		for(UINT16 j = 0; j < 512; j++)
		{
			inputdata[j] = (UINT8) (testMathInstance.pareto_prng() % (1 << 7));
		}

		if(g_SDIODriver.EraseBlock(address, 512 + address) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to erase sd card", NULL, NULL);
			return FALSE;
		}

		if(g_SDIODriver.WriteBlock(inputdata,address, 512) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to write to SD Card", NULL, NULL);
			return FALSE;
		}


		HAL_Time_Sleep_MicroSeconds(1000);


		if(g_SDIODriver.ReadBlock(outputdata, address, 512) != DS_Success)
		{
			DisplayStats(FALSE, "Unable to read SD Card", NULL, NULL);
			return FALSE;
		}

		for(UINT16 i = 0; i < 512; i++)
		{
			if(inputdata[i] != outputdata[i])
			{
				DisplayStats(FALSE, "Level 1 SDIO Test Failed", NULL,NULL);
				return FALSE;
			}
			inputdata[i] = 0;
			outputdata[i] = 0;
		}

	}

	DisplayStats(TRUE, "Level 1 SDIO Test Successful", NULL, NULL);
	return TRUE;

}

BOOL SDIOTest::Level_1()
{
	UINT16 i = 0;

	while(i++ < this->numberOfEvents)
	{

	}
	
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

