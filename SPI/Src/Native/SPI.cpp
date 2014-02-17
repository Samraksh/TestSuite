////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SPI.h"


//---//


SPI::SPI( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	this->numberOfEvents = numberOfEvents;
	testMathInstance.prng_init(seedValue);
};

BOOL SPI::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

/* This test toggles a GPIO pin high and then low */
BOOL SPI::Level_0A()
{
	CPU_GPIO_Uninitialize();
	CPU_SPI_Initialize();

	// Stores the configuration of the spi
	SPI_CONFIGURATION config;
	config.DeviceCS			= 10;
	config.CS_Active		= false;
	config.CS_Setup_uSecs	= 0;
	config.CS_Hold_uSecs	= 0;
	config.MSK_IDLE			= false;
	config.MSK_SampleEdge	= false;
	config.Clock_RateKHz	= 2;
	config.MD_16bits		= false;
	config.SPI_mod			= SPIBUS2;		//Needs to change based on type of radio (LR vs regular)

	return TRUE;
}

BOOL SPI::Level_0B()
{
}




BOOL SPI::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0;
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	default:
		break;
	}

} //Execute

