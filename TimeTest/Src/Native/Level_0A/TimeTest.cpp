////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "TimeTest.h"
#include <Samraksh\HAL_util.h>

//---//



TimeTest::TimeTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	// Initialize the time driver
	HAL_Time_Initialize();

	this->numberOfEvents = numberOfEvents;

	testMathInstance.prng_init(30);

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);

};

BOOL TimeTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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
BOOL TimeTest::Level_0A()
{

	UINT16 i = 0;

	UINT64 currentTicks = 0;
	UINT64 lastReadTicks = 0;

	UINT32 snapshot  = 0;

	// HAL_Time_CurrentTicks test
	while(i++ < this->numberOfEvents)
	{

		hal_printf("Ticks : %u\n", snapshot);

		currentTicks = HAL_Time_CurrentTicks();

		snapshot = (UINT32) currentTicks;

		HAL_Time_Sleep_MicroSeconds(300);

		if(currentTicks < lastReadTicks)
		{
			hal_printf("The failure snapshot is %u\n", snapshot);
			DisplayStats(FALSE,"Time Test Level 0 Failed", "HAL_Time_CurrentTicks is rolling over",0);
			return FALSE;
		}

		lastReadTicks = currentTicks;

	}

	// Reset i
	i = 0;

	INT64 currentTime = 0;
	INT64 lastReadTime = 0;

	// HAL_Time_CurrentTime test
	while(i++ < this->numberOfEvents)
	{
		hal_printf("Time : %u\n", snapshot);

		currentTime = HAL_Time_CurrentTime();

		snapshot = (UINT32) currentTime;

		HAL_Time_Sleep_MicroSeconds(300);

		if(currentTime < lastReadTime)
		{
			DisplayStats(FALSE,"Time Test Level 0 Failed","HAL_Time_CurrentTime is rolling over", 0);
			return FALSE;
		}

		lastReadTime = currentTime;
	}

	DisplayStats(TRUE,"Time Test Level 0 Succeeded","",0);

	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL TimeTest::Level_0B()
{
	return TRUE;

}

BOOL TimeTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL TimeTest::Level_0C()
{
	return TRUE;
}


BOOL TimeTest::Execute( int testLevel )
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

