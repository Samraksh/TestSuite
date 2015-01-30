////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "AdvancedTimerTest.h"
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

BOOL TimeTest::Level_0B()
{
	UINT64 prevTicks = 0, currentTicks = 0;
	//static int counterTestB = 0;

	while(true)
	{
		//counterTestB++;
		//if(counterTestB >= 152081)
			//int i = 0;
		prevTicks = HAL_Time_CurrentTicks();
		HAL_Time_Sleep_MicroSeconds(1000);
		currentTicks = HAL_Time_CurrentTicks();

		if(currentTicks > prevTicks)
			Timer_0_Handler(NULL);
		else
		{
			Timer_1_Handler(NULL);
			debug_printf("prevTicks: %llu; currentTicks: %llu \n", prevTicks, currentTicks);
			//break;
		}
	}

	return TRUE;
}


BOOL TimeTest::Level_0C()
{
	UINT64 prevTime = 0, currentTime = 0, elapsedTime = 0;

	UINT32 timeout_ms =  100;
	//700 is purely from observation while debugging.
	UINT32 ticksScalingFactor = 0x4FD58;
	//convert timeout_ms to ticks for comparison with timeElapsed
	UINT64 timeout_ticks = CPU_MicrosecondsToTicks(timeout_ms * 1000);
	UINT32 events = 0;
	const UINT32 c_EventsMask =  SYSTEM_EVENT_FLAG_USB_IN |
												   SYSTEM_EVENT_FLAG_BUTTON;

	while(true)
	{
		prevTime = HAL_Time_CurrentTime();			//returns ticks value

		events = ::Events_WaitForEvents( c_EventsMask, timeout_ms );		//timeout is in milliseconds

		currentTime = HAL_Time_CurrentTime();
		elapsedTime = currentTime - prevTime;

		if(events != 0)
		{
			 Events_Clear( events );
		}

		if(elapsedTime <= (timeout_ticks + ticksScalingFactor))
			Timer_0_Handler(NULL);
		else if(elapsedTime > (timeout_ticks + ticksScalingFactor))
			Timer_1_Handler(NULL);
	}

	return TRUE;
}

BOOL TimeTest::Level_0D()
{
	UINT64 prevTime = 0, currentTime = 0, elapsedTime = 0;
	UINT16 ticksScalingFactor = 10000;
	UINT32 sleepValue = 100;
	UINT16 incrementVal = 100;

	while(true)
	{
		//sleepValue = (UINT32)(testMathInstance.pareto_prng() % (1 << 7));
		UINT64 sleepValue_ticks = CPU_MicrosecondsToTicks(sleepValue);
		prevTime = HAL_Time_CurrentTime();			//returns ticks value

		HAL_Time_Sleep_MicroSeconds(sleepValue);

		currentTime = HAL_Time_CurrentTime();
		elapsedTime = currentTime - prevTime;

		if(elapsedTime <= (sleepValue_ticks + ticksScalingFactor))
			Timer_0_Handler(NULL);
		else if(elapsedTime > (sleepValue_ticks + ticksScalingFactor))
		{
			Timer_1_Handler(NULL);
			//debug_printf("prevTime: %llu; currentTime: %llu; elapsedTime: %llu \n", prevTime, currentTime, elapsedTime);
		}

		sleepValue += incrementVal;
		//ticksScalingFactor += 50;
	}

	return TRUE;
}


BOOL TimeTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
	else if(testLevel == 2)
		return Level_0C();
	else if(testLevel == 3)
		return Level_0D();
} //Execute


void TimeCallback(void *arg)
{
	Timer_0_Handler(NULL);
	timeTest.Level_0B();
}


