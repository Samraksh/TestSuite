////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "HALTimerTest.h"
#include <Samraksh\HALTimer.h>

extern HALTimerManager gHalTimerManagerObject;
//---//


void Timer_2_Handler(void *arg)
{
	static bool pinState2 = false;
	if (pinState2 == false)
		pinState2 = true;
	else 
		pinState2 = false;
	CPU_GPIO_SetPinState((GPIO_PIN) 25, pinState2);
}

void Timer_1_Handler(void *arg)
{
	static bool pinState = false;

	if (pinState == false)
		pinState = true;
	else 
		pinState = false;
	CPU_GPIO_SetPinState((GPIO_PIN) 24, pinState);
}

HALTimerTest::HALTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

	Tasklet_Initialize();

	gHalTimerManagerObject.Initialize();
};

BOOL HALTimerTest::Level_0A()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 200000, FALSE, FALSE, Timer_2_Handler);

	gHalTimerManagerObject.StartTimer(1);
	gHalTimerManagerObject.StartTimer(2);
	HAL_Time_Sleep_MicroSeconds(1000000);
	
	return TRUE;
}

BOOL HALTimerTest::Level_0B()
{

	return TRUE;
}




BOOL HALTimerTest::Execute( int testLevel )
{
	Level_0A();
} 

