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
	CPU_GPIO_SetPinState((GPIO_PIN) 25, pinState);
}

void Timer_4_Handler(void *arg)
{
	GLOBAL_LOCK(irq);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	for(volatile UINT32 i = 0; i < 200000; i++);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

	ENABLE_INTERRUPTS();

}

GlobalLockTest::GlobalLockTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
};

BOOL GlobalLockTest::Execute( int testLevel )
{
	if(!VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(4, 0, 120000, FALSE, FALSE, Timer_4_Handler))
		return FALSE;

	VirtTimer_Start( 3 );
	VirtTimer_Start( 4 );

	return TRUE;
} //Execute

