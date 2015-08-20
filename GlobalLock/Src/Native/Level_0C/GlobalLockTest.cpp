////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GlobalLockTest.h"
#include <Samraksh\VirtualTimer.h>

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

GlobalLockTest::GlobalLockTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);
};

void toggleGPIO()
{	
	static bool pinState2 = false;

	GLOBAL_LOCK(irq);

	if (pinState2 == false)
		pinState2 = true;
	else 
		pinState2 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, pinState2);
}

BOOL GlobalLockTest::Execute( int testLevel )
{
	if(!VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler))
		return FALSE;

	VirtTimer_Start( 3 );

	while(TRUE)
	{
		{
			toggleGPIO();

			CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
			for(volatile UINT32 i = 0; i < 50000; i++ );
			CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
		}
		::Events_WaitForEvents( 0, 100 );
	}

	return TRUE;
} //Execute

