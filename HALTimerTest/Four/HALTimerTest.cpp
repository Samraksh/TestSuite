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
	static int handlerCount = 0;
	static bool pinState = false;

	handlerCount++;
	if (pinState == false)
		pinState = true;
	else 
		pinState = false;
	CPU_GPIO_SetPinState((GPIO_PIN) 24, pinState);
}

void Timer_3_Handler(void *arg)
{
	static bool pinState3 = false;
	if (pinState3 == false)
		pinState3 = true;
	else 
		pinState3 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 29, pinState3);
}

void Timer_4_Handler(void *arg)
{
	static bool pinState4 = false;
	if (pinState4 == false)
		pinState4 = true;
	else 
		pinState4 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 30, pinState4);

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
	gHalTimerManagerObject.CreateTimer(1, 0, 50000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 125000, FALSE, FALSE, Timer_2_Handler);
	gHalTimerManagerObject.CreateTimer(3, 0, 200000, FALSE, FALSE, Timer_3_Handler);
	gHalTimerManagerObject.CreateTimer(4, 0, 600000, FALSE, FALSE, Timer_4_Handler);

	gHalTimerManagerObject.StartTimer(1);
	gHalTimerManagerObject.StartTimer(2);
	gHalTimerManagerObject.StartTimer(3);
	gHalTimerManagerObject.StartTimer(4);
	
	return TRUE;
}

BOOL HALTimerTest::Level_0B()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 25000, FALSE, FALSE, Timer_2_Handler);
	gHalTimerManagerObject.CreateTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler);
	gHalTimerManagerObject.CreateTimer(4, 0, 500, FALSE, FALSE, Timer_4_Handler);

	return TRUE;
}




BOOL HALTimerTest::Execute( int testLevel )
{
	Level_0A();
} 

