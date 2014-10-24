////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// Author: Mukundan Sridharan
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
#include "data.h"
#include <stm32f10x.h>

extern const int dataLength;
extern int testData[dataLength];

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
    int i;
	int usThreadSleep = 2500;

	CPU_GPIO_EnableOutputPin (24, TRUE);
	CPU_GPIO_EnableOutputPin (25, TRUE);

	// processing algorithm goes here
	for (i=0; i<dataLength; i++)
	{
		testData[i] = testData[i] + 1;
	}

	// if processing takes too long we have to wait for TestRig to open COM port and start listening before we send back processed data
	CPU_GPIO_SetPinState( 25, TRUE);
	for (i=0; i<3000; i++){
		HAL_Time_Sleep_MicroSeconds(usThreadSleep);
	}
	CPU_GPIO_SetPinState( 25, FALSE);

	// sending output back to TestRig
	for (i=0; i<dataLength; i++)
	{
		hal_printf("%d\r\n",testData[i]);
	}

	// done
	while (true){
		CPU_GPIO_SetPinState( 24, TRUE);
		CPU_GPIO_SetPinState( 24, FALSE);
	}
}

