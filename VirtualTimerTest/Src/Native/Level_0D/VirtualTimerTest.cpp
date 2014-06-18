////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
//TODO: AnanthAtSamraksh -- below 2 includes need to be fixed
#include "D:/AnanthAtSamraksh/MF/MicroFrameworkPK_v4_3/DeviceCode/Include/Samraksh/VirtualTimer.h"

//extern HALTimerManager gHalTimerManagerObject;
//---//

BOOL timerToggle = FALSE;

void Timer_1_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_2_Handler(void *arg)
{
	if(timerToggle == FALSE)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
		VirtTimer_Change(1, 0, 30000, FALSE);
		timerToggle = TRUE;
	}
	else
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
		VirtTimer_Change(1, 0, 20000, FALSE);
		timerToggle = FALSE;
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

	if(!VirtTimer_Initialize())
		hal_printf("Error");
};

BOOL VirtualTimerTest::Level_0A()
{
	if(!VirtTimer_SetTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler))
		return FALSE;

	return TRUE;
}

// Creates two timers
BOOL VirtualTimerTest::Level_0B()
{
	if(!VirtTimer_SetTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(2, 0, 30000, FALSE, FALSE, Timer_2_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(3, 0, 40000, FALSE, FALSE, Timer_3_Handler))
		return FALSE;

	return TRUE;
}


BOOL VirtualTimerTest::Level_0C()
{
	if(!VirtTimer_SetTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(2, 0, 110000, FALSE, FALSE, Timer_2_Handler))
		return FALSE;
}


BOOL VirtualTimerTest::Level_0D()
{
	if(!VirtTimer_SetTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler))
		return FALSE;
	if(!VirtTimer_SetTimer(2, 0, 110000, FALSE, FALSE, Timer_2_Handler))
		return FALSE;

	if(!VirtTimer_Start(1))
		return FALSE;

	if(!VirtTimer_Start(2))
		return FALSE;

	return TRUE;
}

BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
	else if(testLevel == 2)
		return Level_0C();
	else if(testLevel == 3)
		return Level_0D();
} //Execute

