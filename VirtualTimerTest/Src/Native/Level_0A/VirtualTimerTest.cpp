////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include <Samraksh\VirtualTimer.h>

extern VirtualTimerManager gVirtualTimerManagerObject;
//---//
void Timer_1_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_2_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
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


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 22, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 23, FALSE);
	Tasklet_Initialize();

	gVirtualTimerManagerObject.Initialize();
};

BOOL VirtualTimerTest::Level_0A()
{
	gVirtualTimerManagerObject.CreateTimer(1, 0, 10000, FALSE, FALSE, Timer_1_Handler);

	while(TRUE)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
		::Events_WaitForEvents( 0, 100 );
		CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	}


	return TRUE;
}

BOOL VirtualTimerTest::Level_0B()
{
	gVirtualTimerManagerObject.CreateTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler);
	gVirtualTimerManagerObject.CreateTimer(2, 0, 25000, FALSE, FALSE, Timer_2_Handler);
	gVirtualTimerManagerObject.CreateTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler);
	gVirtualTimerManagerObject.CreateTimer(4, 0, 500, FALSE, FALSE, Timer_4_Handler);

	return TRUE;
}




BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
} //Execute

