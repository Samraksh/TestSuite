////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This is a test for profiling the AdvancedTimer driver. */

#include "AdvancedTimerTest.h"
//TODO: AnanthAtSamraksh -- below include needs to be fixed
#include "D:/AnanthAtSamraksh/MF/MicroFrameworkPK_v4_3/DeviceCode/Include/Samraksh/VirtualTimer.h"
#include "D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\Krait\DeviceCode\Krait_TIME\Krait__TIME.h"
#include "D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Targets\Native\Krait\DeviceCode\Krait_TIMER\Krait__TIMER.h"


void VirtualTimerCallback(void *arg);
AdvancedTimerTest AdvTimertest(0,0);

extern Krait_Timer g_Krait_Timer;

//---//

/*enum SetCompareType {
	SET_COMPARE_TIMER,
	SET_COMPARE_COMPLETION
};*/

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



AdvancedTimerTest::AdvancedTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 52, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 55, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 58, TRUE);
	/*CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);*/

	//Tasklet_Initialize();

	g_Krait_Timer.InitializeTimer(0, VirtualTimerCallback, NULL);
};

BOOL AdvancedTimerTest::Level_0A()
{
	UINT32 uSec = 500000;
	UINT16 Timer = 0;
	g_Krait_Timer.SetCompare(0, CPU_MicrosecondsToTicks(uSec, Timer));

	return TRUE;
}



BOOL AdvancedTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute


void VirtualTimerCallback(void *arg)
{
	Timer_0_Handler(NULL);
	AdvTimertest.Level_0A();
}


