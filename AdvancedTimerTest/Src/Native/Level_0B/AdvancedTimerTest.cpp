////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "AdvancedTimerTest.h"
#include "..\DeviceCode\Include\time_decl.h"

//extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
void AdvancedTimerCallback(void *arg);
AdvancedTimerTest advTest(0,0);

UINT32 AdvancedTimerTest::prevTicks = 0;

//---//

/*enum SetCompareType {
	SET_COMPARE_TIMER,
	SET_COMPARE_COMPLETION
};*/


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


BOOL AdvancedTimerTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

AdvancedTimerTest::AdvancedTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);

	//Tasklet_Initialize();

	//g_STM32F10x_AdvancedTimer.Initialize(0, VirtualTimerCallback, NULL);
	CPU_Timer_Initialize(1, FALSE, 0, AdvancedTimerCallback, NULL);
};


UINT64 MicrosecondsToTicks( UINT64 uSec ) {
	return uSec * (8000000/1000000);
}


BOOL AdvancedTimerTest::Level_0A()
{
	CPU_Timer_SetCompare(1, CPU_MicrosecondsToTicks((UINT32)10000));
}


BOOL AdvancedTimerTest::Level_0B()
{
	currentTicks = 0;
	UINT32 i = 0;

	//for(UINT64 i = 0; i < 2^33; ++i)
	while(currentTicks >= 0 && currentTicks < ((1<<32)-1))
	{
		currentTicks = CPU_Timer_CurrentTicks(1);
		if(currentTicks < 0 || currentTicks < prevTicks)
		{
			if(prevTicks > ((1<<31)-1))
			{
				hal_printf("count: %llu currentTicks > prevTicks \r\n", i);
				hal_printf("currentTicks: %u\r\n", currentTicks);
				break;
			}
			else
			{
				//Timer_0_Handler(NULL);
				DisplayStats(false, "ERROR: currentTicks is less than prevTicks", NULL, 0);
				return false;
			}
		}
		else
		{
			//Timer_1_Handler(NULL);
			if(!(i % 10000000))
			{
				hal_printf("count: %llu currentTicks > prevTicks \r\n", i);
				hal_printf("currentTicks: %u\r\n", currentTicks);
			}
		}
		prevTicks = currentTicks;
		i++;
	}

	DisplayStats(true, "SUCCESS: currentTicks is always greater than prevTicks", NULL, 0);
}


BOOL AdvancedTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
} //Execute


void AdvancedTimerCallback(void *arg)
{
	Timer_0_Handler(NULL);
	advTest.Level_0A();
}


