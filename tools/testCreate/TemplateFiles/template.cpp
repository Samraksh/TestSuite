////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "TEMPLATE.h"
#include <Samraksh\HAL_util.h>

//---//


TEMPLATE::TEMPLATE(  )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

};

BOOL TEMPLATE::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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
BOOL TEMPLATE::Level_0A()
{

	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL TEMPLATE::Level_0B()
{
	return TRUE;

}

BOOL TEMPLATE::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL TEMPLATE::Level_0C()
{
	return TRUE;
}


BOOL TEMPLATE::Execute( int testLevel )
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

