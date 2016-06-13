////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Continuations.h"
#include <Samraksh\VirtualTimer.h>

#define VT_CALLBACK_CONTINUATION_MAX 8
static HAL_CONTINUATION vtCallbackContinuationArray[VT_CALLBACK_CONTINUATION_MAX];

bool queueVTCallback(HAL_CALLBACK_FPN callback){
	int i;
	
	for (i=0; i<VT_CALLBACK_CONTINUATION_MAX; i++){
		if (!vtCallbackContinuationArray[i].IsLinked())
		{
			vtCallbackContinuationArray[i].InitializeCallback((HAL_CALLBACK_FPN) (callback),NULL);   
			vtCallbackContinuationArray[i].Enqueue();
			return true;
		}
	}
	return false;
}
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
	static bool pinState4 = false;

	if (pinState4 == false)
		pinState4 = true;
	else 
		pinState4 = false;
	CPU_GPIO_SetPinState((GPIO_PIN) 25, pinState4);

}

GlobalLockTest::GlobalLockTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
};

BOOL GlobalLockTest::Execute( int testLevel )
{
	int i;
	while (true){
		queueVTCallback(Timer_3_Handler);
		queueVTCallback(Timer_4_Handler);

		::Events_WaitForEvents( 0, 1 );
	}

	return TRUE;
} //Execute

