////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hal_time_sleep.h"
#include "Parameters.h"
#include <stm32f10x.h>

//---//


haltimesleepTest::haltimesleepTest( int seedValue, int numberOfEvents )
{

};

BOOL haltimesleepTest::Execute( int testLevel )
{
	CPU_GPIO_EnableOutputPin(24,TRUE);
	CPU_GPIO_SetPinState(24, TRUE);

	int usThreadSleep = (int)( (1/(float)frequency)/2 * 1000000);
	while (true){
		CPU_GPIO_SetPinState( 24, TRUE);
		HAL_Time_Sleep_MicroSeconds(usThreadSleep);
		CPU_GPIO_SetPinState( 24, FALSE);
		HAL_Time_Sleep_MicroSeconds(usThreadSleep);
	}
	return TRUE;
} //Execute

