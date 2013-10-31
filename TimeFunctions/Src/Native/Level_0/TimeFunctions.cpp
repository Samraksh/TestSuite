////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TimeFunctions.h"
#include <Samraksh\HAL_util.h>

//---//


TimeFunctions::TimeFunctions( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	testMathInstance.prng_init(30);

};

BOOL TimeFunctions::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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
BOOL TimeFunctions::Level_0A()
{

	UINT32 buffer32[32];

	for(UINT16 i = 0 ; i < 32; i++)
	{
		buffer32[i] = 0;
	}



	UINT32 temp = 0;

	UINT64 buffer64[64];

	for(UINT16 i = 0 ;  i < 64; i++)
	{
		buffer64[i] = 0;
	}

	// Check if the system clock is equal to SYSTEM_CLOCK_HZ
	if(CPU_SystemClock() != SYSTEM_CLOCK_HZ)
	{
		DisplayStats(FALSE,"Clock Mismatch",NULL, 0);
	}

	// Test for the ticks per second api
	if(CPU_TicksPerSecond() != SYSTEM_CLOCK_HZ)
	{
		DisplayStats(FALSE,"Configuration Mismatch", NULL, 0);
	}

	for(UINT16 i = 0 ; i < 1000; i++)
	{
		timeForTi
		UINT64 ticks = CPU_MillisecondsToTicks((UINT32) testMathInstance.prng());
	}



#if 0
	for(UINT16 i = 1; i < 32; i++)
	{
		timeForTicks[i] = ((8000 * i) * 1000) / SYSTEM_CLOCK_HZ;
		temp = CPU_TicksToTime((UINT32)(8000 * i));
		if(temp != timeForTicks[i])
		{
			DisplayStats(FALSE,"CPU_TicksToTime Api failed test", NULL, 0);
			return FALSE;
		}
	}

	for(UINT16 i = 1; i < 64; i++)
	{
		timeForTicks64[i] = (1 << i) * (1 / SYSTEM_CLOCK_HZ);
		if(CPU_TicksToTime((UINT64) (1 << i)) != timeForTicks[i])
		{
			DisplayStats(FALSE,"CPU_TicksToTime 64 bit Api failed test", NULL, 0);
			return FALSE;
		}
	}


    for(UINT32 i = 1,j = 0; j < 8; i *= 10, j++)
    {
    	timeForTicks[j] = i * (SYSTEM_CLOCK_HZ/1000);
    	if(CPU_MillisecondsToTicks(i) != timeForTicks[i])
    	{
    		DisplayStats(FALSE,"CPU_MilliseconsToTicks api failed",NULL, 0);
    	}

    }

    for(UINT64 i = 1,j = 0; j < 16; i *= 10, j++)
    {
      	timeForTicks64[j] = i * (SYSTEM_CLOCK_HZ/1000);
       	if(CPU_MillisecondsToTicks(i) != timeForTicks64[i])
       	{
       		DisplayStats(FALSE,"CPU_MilliseconsToTicks api failed",NULL, 0);
       	}
    }
#endif

    DisplayStats(TRUE, "Time function test passed", NULL, 0);


	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL TimeFunctions::Level_0B()
{
		return TRUE;

}

BOOL TimeFunctions::Level_1()
{
		return TRUE;
}

// Tests the buffer write and read interface
BOOL TimeFunctions::Level_0C()
{
		return TRUE;
}


BOOL TimeFunctions::Execute( int testLevel )
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

