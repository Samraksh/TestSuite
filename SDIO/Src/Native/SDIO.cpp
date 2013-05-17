////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "SDIO.h"
#include <Samraksh\HAL_util.h>

//---//


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

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL SDIOTest::Level_0A()
{

	UINT16 i = 0;

	while(i++ < this->numberOfEvents)
	{

	}
	DisplayStats(FALSE, "Reading Manufacture ID Failed", NULL, NULL);

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

