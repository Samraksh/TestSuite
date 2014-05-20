////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include <Samraksh\HALTimer.h>

extern HALTimerManager gHalTimerManagerObject;
//---//

/*void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
}*/

void Timer_2_Handler(void *arg)
{
	//GLOBAL_LOCK(irq);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	//HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1000000);
	for(volatile UINT32 i = 0; i < 100000; i++);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

	ENABLE_INTERRUPTS();

}

void Timer_1_Handler(void *arg)
{
	static int handlerCount = 0;

	handlerCount++;
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	if ((handlerCount%50)==0){
		gHalTimerManagerObject.StartTimer(2);
	}
	if ((handlerCount%100)==0){
		HAL_Time_Sleep_MicroSeconds(1000000);
	}
}

void Timer_3_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
}

void Timer_4_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
}


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

	Tasklet_Initialize();

	gHalTimerManagerObject.Initialize();
};

BOOL VirtualTimerTest::Level_0A()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);

	return TRUE;
}

// Creates two timers
BOOL VirtualTimerTest::Level_0B()
{
	/*gHalTimerManagerObject.CreateTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 30000, FALSE, FALSE, Timer_2_Handler);
	gHalTimerManagerObject.CreateTimer(3, 0, 40000, FALSE, FALSE, Timer_3_Handler);*/

	gHalTimerManagerObject.CreateTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 2000000, TRUE, FALSE, Timer_2_Handler);

	HAL_Time_Sleep_MicroSeconds(1000000);
	gHalTimerManagerObject.StartTimer(1);
	gHalTimerManagerObject.StartTimer(2);

	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
} //Execute

