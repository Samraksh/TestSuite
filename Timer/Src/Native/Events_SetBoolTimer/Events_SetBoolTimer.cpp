////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Events_SetBoolTimer.h"
#include <Samraksh\HALTimer.h>

extern HALTimerManager gHalTimerManagerObject;
//---//




HALTimerTest::HALTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);

	Tasklet_Initialize();

	gHalTimerManagerObject.Initialize();
};

BOOL HALTimerTest::Level_0A()
{
	
	return TRUE;
}

BOOL HALTimerTest::Level_0B()
{

	return TRUE;
}

BOOL HALTimerTest::Execute( int testLevel )
{
	UINT32 frequency = 5;
	UINT32 wait = (UINT32)( (1/frequency)/2 * 1000);
	BOOL m_timeQuantumExpired;
	
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);

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

