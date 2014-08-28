////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GlobalLockTest.h"
#include <Samraksh\VirtualTimer.h>

//extern HALTimerManager gHalTimerManagerObject;
//---//
void Timer_3_Handler(void *arg)
{
	static bool pinState = false;

	if (pinState == false)
		pinState = true;
	else 
		pinState = false;
	CPU_GPIO_SetPinState((GPIO_PIN) 24, pinState);
}

void Timer_4_Handler(void *arg)
{
	GLOBAL_LOCK(irq);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	for(volatile UINT32 i = 0; i < 100000; i++);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

	ENABLE_INTERRUPTS();

}

void Timer_5_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
}

void Timer_6_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
}


GlobalLockTest::GlobalLockTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);

	//Tasklet_Initialize();

	//gHalTimerManagerObject.Initialize();
};

BOOL GlobalLockTest::Level_0A()
{
	//gHalTimerManagerObject.CreateTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);
	//gHalTimerManagerObject.CreateTimer(2, 0, 120000, FALSE, FALSE, Timer_2_Handler);

	if(!VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(4, 0, 120000, FALSE, FALSE, Timer_4_Handler))
		return FALSE;

	return TRUE;
}

void foo()
{	
	static bool pinState2 = false;

	GLOBAL_LOCK(irq);

	if (pinState2 == false)
		pinState2 = true;
	else 
		pinState2 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, pinState2);
}

BOOL GlobalLockTest::Level_0B()
{
	/*gHalTimerManagerObject.CreateTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.StartTimer(1);*/

	if(!VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler))
		return FALSE;

	VirtTimer_Start( 3 );

	while(TRUE)
	{
		{
			GLOBAL_LOCK(irq);

			foo();

			CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
			for(volatile UINT32 i = 0; i < 50000; i++ );
			CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
		}
	}

	return TRUE;
}

BOOL GlobalLockTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
} //Execute

