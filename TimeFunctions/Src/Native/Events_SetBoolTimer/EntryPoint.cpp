////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// Author: Mukundan Sridharan
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
#include "Parameters.h"
#include <i2c/stm32f10x_i2c.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}


HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{
	UINT32 wait = (UINT32)( 500 / frequency);
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

