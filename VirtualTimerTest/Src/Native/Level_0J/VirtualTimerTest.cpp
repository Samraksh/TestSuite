////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"



//---//
static int counter;
VirtualTimerTest vTest(0, 0);

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
	vTest.DisplayStats(true, "SUCCESS: Timer started with max delay", NULL, 0);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
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
BOOL VirtualTimerTest::Level_0J()
{
	if(!VirtTimer_SetTimer(2, 0, 2000000, TRUE, FALSE, Timer_2_Handler))
		return FALSE;

	//Use either 0xFFFFFFFF or (2^32)-35 to get the max timer period. Anything higher, rolls-over. Period is UINT32.
	//At 8 MHz, 0xFFFFFFFF takes 8'56''
	//if(!VirtTimer_SetTimer(3, 0, (2^32)-35, TRUE, FALSE, Timer_3_Handler))
	if(!VirtTimer_SetTimer(3, 0, 0xFFFFFFFF, TRUE, FALSE, Timer_3_Handler))
		return FALSE;

	if(!VirtTimer_SetTimer(4, 0, 2000000, TRUE, FALSE, Timer_4_Handler))
		return FALSE;

	/*if(!VirtTimer_Start(4))
		return FALSE;*/

	if(!VirtTimer_Start(3))
		return FALSE;

	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0J();
} //Execute

