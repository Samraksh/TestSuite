////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* Test to verify if time works properly.
 * This test checks if sleep works properly or not. */

#include "ElapsedTime.h"
#include "..\DeviceCode\Include\Time_decl.h"

//---//

void Timer_0_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 31, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 31, FALSE);
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

BOOL TimeTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	while(true){
		hal_printf("result=%s\n", (result) ? "PASS":"FAIL");
		hal_printf("accuracy=%d\n", accuracy);
		hal_printf("resultParameter1=%s\n", resultParameter1);
		hal_printf("resultParameter2=%s\n", resultParameter2);
		hal_printf("resultParameter3=null\n");
		hal_printf("resultParameter4=null\n");
		hal_printf("resultParameter5=null\n");
		HAL_Time_Sleep_MicroSeconds(1000000);
	}

	return true;
}

TimeTest::TimeTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);

	//testMathInstance.prng_init(3);
};


BOOL TimeTest::Level_0A()
{
	INT64 prevTime = 0, currentTime = 0;

	while(true)
	{
		prevTime = HAL_Time_CurrentTime();
		currentTime = HAL_Time_CurrentTime();

		if(currentTime > prevTime)
			Timer_0_Handler(NULL);
		else
		{
			Timer_1_Handler(NULL);
			break;
		}
	}

	return TRUE;
}


BOOL TimeTest::Level_0B()
{
	UINT64 prevTicks = 0, currentTicks = 0;

	while(true)
	{
		prevTicks = HAL_Time_CurrentTicks();
		currentTicks = HAL_Time_CurrentTicks();

		if(currentTicks > prevTicks)
			Timer_0_Handler(NULL);
		else
		{
			Timer_1_Handler(NULL);
			break;
		}
	}

	return TRUE;
}


BOOL TimeTest::Level_0C()
{
	UINT64 prevTime = 0, currentTime = 0, elapsedTime = 0;

	UINT32 timeout_ms =  100;
	//700 is purely from observation while debugging.
	UINT16 ticksScalingFactor = 700;
	//convert timeout_ms to ticks for comparison with timeElapsed
	UINT64 timeout_ticks = CPU_MillisecondsToTicks(timeout_ms);
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
	UINT16 ticksScalingFactor = 600;
	UINT32 sleepValue = 100;
	UINT16 incrementVal = 751;
	UINT64 sleepValue_ticks = CPU_MicrosecondsToTicks(sleepValue);

	while(sleepValue < 400000)
	{
		//sleepValue = (UINT32)(testMathInstance.pareto_prng() % (1 << 7));

		prevTime = HAL_Time_CurrentTime();			//returns ticks value

		HAL_Time_Sleep_MicroSeconds(sleepValue);

		currentTime = HAL_Time_CurrentTime();
		elapsedTime = currentTime - prevTime;
		
		//	hal_printf("eT: %llu\r\n",elapsedTime);
		//	hal_printf("sV: %llu\r\n",  sleepValue_ticks);
		Timer_0_Handler(NULL);
		if(elapsedTime <= (sleepValue_ticks))
		{
			//Timer_0_Handler(NULL);
			Timer_2_Handler(NULL);
			
			DisplayStats(false, "ERROR: elapsedTime is less than sleep time", NULL, 0);
		}
		else if(elapsedTime > (sleepValue_ticks))
		{
			Timer_1_Handler(NULL);

		}

		sleepValue += incrementVal;
		sleepValue_ticks = CPU_MicrosecondsToTicks(sleepValue);
	}

	DisplayStats(true, "SUCCESS: elapsedTime is always greater than sleep time", NULL, 0);

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

