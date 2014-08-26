////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "AdvancedTimerTest.h"
#include "..\DeviceCode\Include\time_decl.h"

//extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
void AdvancedTimerCallback(void *arg);
AdvancedTimerTest advTest(0,0);

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
	//g_STM32F10x_AdvancedTimer.SetCompare(g_STM32F10x_AdvancedTimer.Get64Counter(), MicrosecondsToTicks(250000), SET_COMPARE_TIMER);
	//The below line makes the timer fire only once.
	//for (int i = 0; i < 5; i++)
		//g_STM32F10x_AdvancedTimer.SetCompare(g_STM32F10x_AdvancedTimer.GetCounter(), MicrosecondsToTicks(10000), SET_COMPARE_TIMER);
		CPU_Timer_SetCompare(1, CPU_MicrosecondsToTicks((UINT32)10000));
		//g_STM32F10x_AdvancedTimer.SetCompare(0, MicrosecondsToTicks(10000), SET_COMPARE_TIMER);		//Fires just once and the interval is 2.355ms
	//return TRUE;
}


BOOL AdvancedTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute


void AdvancedTimerCallback(void *arg)
{
	Timer_0_Handler(NULL);
	advTest.Level_0A();
}


