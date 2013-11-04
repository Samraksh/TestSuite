////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GlobalLockTest.h"
#include <Samraksh\HALTimer.h>

extern HALTimerManager gHalTimerManagerObject;
//---//
void Timer_1_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_2_Handler(void *arg)
{
	GLOBAL_LOCK(irq);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	for(volatile UINT32 i = 0; i < 100000; i++);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

	ENABLE_INTERRUPTS();

}

void Timer_3_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
}

void Timer_4_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
}


GlobalLockTest::GlobalLockTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

	Tasklet_Initialize();

	gHalTimerManagerObject.Initialize();
};

BOOL GlobalLockTest::Level_0A()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 120000, FALSE, FALSE, Timer_2_Handler);

	return TRUE;
}

BOOL GlobalLockTest::Level_0B()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 25000, FALSE, FALSE, Timer_2_Handler);
	gHalTimerManagerObject.CreateTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler);
	gHalTimerManagerObject.CreateTimer(4, 0, 500, FALSE, FALSE, Timer_4_Handler);

	return TRUE;
}




BOOL GlobalLockTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute
