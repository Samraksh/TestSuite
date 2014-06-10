////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Events_SetBoolTimer.h"
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
	//for(volatile UINT32 i = 0; i < 400000; i++);
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

	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
}

void Timer_4_Handler(void *arg)
{

	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
}


Events_SetBoolTimerTest::Events_SetBoolTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

	Tasklet_Initialize();

	gHalTimerManagerObject.Initialize();
};

BOOL Events_SetBoolTimerTest::Level_0A()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 80000, FALSE, FALSE, Timer_1_Handler);

	//HAL_Time_Sleep_MicroSeconds(1000000);
	gHalTimerManagerObject.StartTimer(1);

	
	return TRUE;
}

BOOL Events_SetBoolTimerTest::Level_0B()
{
	gHalTimerManagerObject.CreateTimer(1, 0, 20000, FALSE, FALSE, Timer_1_Handler);
	gHalTimerManagerObject.CreateTimer(2, 0, 25000, FALSE, FALSE, Timer_2_Handler);
	gHalTimerManagerObject.CreateTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler);
	gHalTimerManagerObject.CreateTimer(4, 0, 500, FALSE, FALSE, Timer_4_Handler);

	return TRUE;
}




BOOL Events_SetBoolTimerTest::Execute( int testLevel )
{
	UINT32 wait =  225;
	BOOL m_timeQuantumExpired;

	while (TRUE){
		m_timeQuantumExpired = false;
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);		
		::Events_SetBoolTimer( (BOOL*)&m_timeQuantumExpired, wait );	
		while (m_timeQuantumExpired == false) {}
		m_timeQuantumExpired = false;
		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		::Events_SetBoolTimer( (BOOL*)&m_timeQuantumExpired, wait );	
		while (m_timeQuantumExpired == false) {}
	}
} 

