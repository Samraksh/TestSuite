////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"



//---//
static int counter;

void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	VirtTimer_Start(4);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

	if(counter < 10)
		VirtTimer_Start(2);
}


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

	//Tasklet_Initialize();

	//if(!VirtTimer_Initialize())
		//hal_printf("Error");
};

BOOL VirtualTimerTest::Level_0A()
{
	VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_1_Handler);

	return TRUE;
}

// Creates two timers
BOOL VirtualTimerTest::Level_0H()
{
	if(!VirtTimer_SetTimer(2, 0, 2000000, TRUE, FALSE, Timer_2_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(3, 0, 10000, FALSE, FALSE, Timer_3_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(4, 0, 2000000, TRUE, FALSE, Timer_4_Handler))
		return FALSE;

	if(!VirtTimer_Start(4))
		return FALSE;

	while(counter < 300)
	{
		counter++;
		HAL_Time_Sleep_MicroSeconds(2000000);
	}

	if(!VirtTimer_Start(3))
		return FALSE;

	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0H();
} //Execute

