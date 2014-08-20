////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "HardwareTimerTest.h"
#include "..\DeviceCode\Include\time_decl.h"


void HardwareTimerCallback(void *arg);
HardwareTimerTest hardwareTimertest(0,0);

//extern Krait_Timer g_Krait_Timer;

//---//


void Timer_0_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);
}

void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 55, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 55, FALSE);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 58, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 58, FALSE);
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



HardwareTimerTest::HardwareTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 51, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 52, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 55, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 58, TRUE);

	//Tasklet_Initialize();

	//g_Krait_Timer.InitializeTimer(0, VirtualTimerCallback, NULL);
	CPU_Timer_Initialize(0, FALSE, 0, HardwareTimerCallback, NULL);
};

BOOL HardwareTimerTest::Level_0A()
{
	UINT32 uSec = 500000;
	UINT16 Timer = 0;

	//g_Krait_Timer.SetCompare(0, CPU_MicrosecondsToTicks(uSec, Timer));
	CPU_Timer_SetCompare(0, CPU_MicrosecondsToTicks(uSec, Timer));

	return TRUE;
}



BOOL HardwareTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute


void HardwareTimerCallback(void *arg)
{
	Timer_0_Handler(NULL);
	hardwareTimertest.Level_0A();
}


