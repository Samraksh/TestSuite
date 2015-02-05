////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"

VirtualTimer gVirtualTimerObject;

//---//
BOOL VirtualTimerTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	while(true){
		hal_printf("result=%s\n", (result) ? "PASS":"FAIL");
		hal_printf("accuracy=%d\n", accuracy);
		hal_printf("resultParameter1=%s\n", resultParameter1);
		hal_printf("resultParameter2=%s\n", resultParameter2);
		hal_printf("resultParameter3=null\n");
		hal_printf("resultParameter4=null\n");
		hal_printf("resultParameter5=null\n");
		HAL_Time_Sleep_MicroSeconds(1000000);
	}

	return true;
}

void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	VirtTimer_Start(2);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	VirtTimer_Start(1);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	VirtTimer_Start(4);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	VirtTimer_Start(3);
}

void Timer_5_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
	VirtTimer_Start(6);
}

void Timer_6_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
	VirtTimer_Start(5);
}

void Timer_7_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	VirtTimer_Start(8);
}

void Timer_8_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	VirtTimer_Start(7);
}


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
};

BOOL VirtualTimerTest::Level_0A()
{
	VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_1_Handler);

	return TRUE;
}

// Creates two timers
BOOL VirtualTimerTest::Level_0G()
{
	int timerCount = 1;	//Start with one, because timer for "time" is 0 and is not being created below
	if(VirtTimer_SetTimer(1, 0, 2000000, TRUE, FALSE, Timer_1_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	if(VirtTimer_SetTimer(2, 0, 2000000, TRUE, FALSE, Timer_2_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	if(VirtTimer_SetTimer(3, 0, 2000000, TRUE, FALSE, Timer_3_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	if(VirtTimer_SetTimer(4, 0, 2000000, TRUE, FALSE, Timer_4_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	if(VirtTimer_SetTimer(5, 0, 2000000, TRUE, FALSE, Timer_5_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	if(VirtTimer_SetTimer(6, 0, 2000000, TRUE, FALSE, Timer_6_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	if(VirtTimer_SetTimer(7, 0, 2000000, TRUE, FALSE, Timer_7_Handler) < 0)		//one-shot timer
	{
		DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);
		return FALSE;
	}
	timerCount++;
	//Test is a success if not able to create timer #8.
	if(VirtTimer_SetTimer(8, 0, 1000000, FALSE, FALSE, Timer_8_Handler) < 0)	//one-shot timer
	{
		if(timerCount == gVirtualTimerObject.virtualTimerMapper_0.VTM_countOfVirtualTimers)
		{
			DisplayStats(true, "SUCCESS: Unable to create timer", NULL, 0);
		}
		else
			DisplayStats(false, "ERROR: Unable to create timer", NULL, 0);

		return FALSE;
	}

	if(!VirtTimer_Start(2))
		return FALSE;

	if(!VirtTimer_Start(4))
		return FALSE;

	if(!VirtTimer_Start(6))
		return FALSE;

	if(!VirtTimer_Start(8))
		return FALSE;

	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0G();
} //Execute

