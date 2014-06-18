////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "AdvancedTimerTest.h"
//TODO: AnanthAtSamraksh -- below 2 includes need to be fixed
#include "D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\time_decl.h"

//extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
void VirtualTimerCallback(void *arg);
VirtualTimerTest VTtest(0,0);

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


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);

	Tasklet_Initialize();

	//g_STM32F10x_AdvancedTimer.Initialize(0, VirtualTimerCallback, NULL);
	CPU_Timer_Initialize(0, FALSE, 0, VirtualTimerCallback, NULL);
};

BOOL VirtualTimerTest::Level_0A()
{
	//VirtTimer_SetTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);

	return TRUE;
}

UINT64 MicrosecondsToTicks( UINT64 uSec ) {
	return uSec * (8000000/1000000);
}


BOOL VirtualTimerTest::Level_0F()
{
	//g_STM32F10x_AdvancedTimer.SetCompare(g_STM32F10x_AdvancedTimer.Get64Counter(), MicrosecondsToTicks(250000), SET_COMPARE_TIMER);
	//The below line makes the timer fire only once.
	//for (int i = 0; i < 5; i++)
		//g_STM32F10x_AdvancedTimer.SetCompare(g_STM32F10x_AdvancedTimer.GetCounter(), MicrosecondsToTicks(10000), SET_COMPARE_TIMER);
		CPU_Timer_SetCompare(1, CPU_MicrosecondsToTicks((UINT32)10000));
		//g_STM32F10x_AdvancedTimer.SetCompare(0, MicrosecondsToTicks(10000), SET_COMPARE_TIMER);		//Fires just once and the interval is 2.355ms
	//return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 5)
	{
		Level_0F();
	}
} //Execute


void VirtualTimerCallback(void *arg)
{
	Timer_0_Handler(NULL);
	VTtest.Level_0F();
}


