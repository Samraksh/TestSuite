////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "HALTimerTest.h"
#include <Samraksh\HALTimer.h>

extern HALTimerManager gHalTimerManagerObject;
//---//
void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 0, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 10, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 10, FALSE);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
}


HALTimerTest::HALTimerTest( int seedValue, int numberOfEvents )
{
	// Initialize the timer manager object
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 0, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 10, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 22, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);
	gHalTimerManagerObject.Initialize();
};

BOOL HALTimerTest::Level_0A()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 25000, FALSE, FALSE, Timer_2_Handler);
	gHalTimerManagerObject.CreateTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler);
	gHalTimerManagerObject.CreateTimer(4, 0, 10000, FALSE, FALSE, Timer_4_Handler);
	return TRUE;

}

BOOL HALTimerTest::Level_0B()
{
}




BOOL HALTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute

