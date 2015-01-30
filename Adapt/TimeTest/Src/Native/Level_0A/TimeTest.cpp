////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "TimeTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"


void TimeCallback(void *arg);
TimeTest timeTest(0,0);


//---//

void Timer_0_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 51, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 51, FALSE);
}

void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 55, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 55, FALSE);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 58, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 58, FALSE);
}

void Timer_5_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 2, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 2, FALSE);
}

void Timer_6_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 4, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 4, FALSE);
}

void Timer_7_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
}


TimeTest::TimeTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 51, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 52, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 55, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 58, TRUE);
	/*CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);*/

	//Tasklet_Initialize();

};

BOOL TimeTest::Level_0A()
{
	INT64 prevTime = 0, currentTime = 0;
	UINT64 prevTicks = 0, currentTicks = 0;
	static int counterTestA = 0;

	////debug_printf("currentTime: %llu \r\n", HAL_Time_CurrentTime());

	while(true)
	{
		/*counterTestA++;
		if(counterTestA % 100000 == 0)
			debug_printf("currentTime: %llu \r\n", HAL_Time_CurrentTime());*/

		prevTicks = HAL_Time_CurrentTicks();
		prevTime = HAL_Time_CurrentTime();
		HAL_Time_Sleep_MicroSeconds(2000);
		HAL_Time_Sleep_MicroSeconds(2000);
		HAL_Time_Sleep_MicroSeconds(2000);
		currentTime = HAL_Time_CurrentTime();
		currentTicks = HAL_Time_CurrentTicks();

		if(currentTime > prevTime)
			Timer_0_Handler(NULL);
		else
		{
			Timer_1_Handler(NULL);
			debug_printf("prevTime: %llu; currentTime: %llu \r\n", prevTime, currentTime);
			debug_printf("prevTicks: %llu; currentTicks: %llu \r\n", prevTicks, currentTicks);
			//break;
		}
	}

	return TRUE;
}



BOOL TimeTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute


void TimeCallback(void *arg)
{
	Timer_0_Handler(NULL);
	timeTest.Level_0A();
}


