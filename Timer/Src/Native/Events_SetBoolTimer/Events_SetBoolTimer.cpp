////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Events_SetBoolTimer.h"
#include <Samraksh\HAL_util.h>

//---//


void Timer_2_Handler(void *arg)
{

}

void Timer_1_Handler(void *arg)
{

}

void Timer_3_Handler(void *arg)
{

}

void Timer_4_Handler(void *arg)
{

}


Events_SetBoolTimerTest::Events_SetBoolTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

//	Tasklet_Initialize();

//	gHalTimerManagerObject.Initialize();
};

BOOL Events_SetBoolTimerTest::Level_0A()
{
	
	
	return TRUE;
}

BOOL Events_SetBoolTimerTest::Level_0B()
{

	return TRUE;
}




BOOL Events_SetBoolTimerTest::Execute( int testLevel )
{
	UINT32 wait =123;
	volatile BOOL m_timeQuantumExpired;
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

