////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"
#include "../DeviceCode/Include/Time_decl.h"
#include "platform_selector.h"

//---//

void Timer_0_Handler(void *arg)
{
	static bool pinState0 = false;

	if (pinState0 == false)
		pinState0 = true;
	else 
		pinState0 = false;

	CPU_GPIO_SetPinState(TEST_PIN0, pinState0);
}

void Timer_1_Handler(void *arg)
{
	static bool pinState1 = false;

	if (pinState1 == false)
		pinState1 = true;
	else 
		pinState1 = false;

	CPU_GPIO_SetPinState(TEST_PIN1, pinState1);
}

void Timer_2_Handler(void *arg)
{
	static bool pinState2 = false;

	if (pinState2 == false)
		pinState2 = true;
	else 
		pinState2 = false;

	CPU_GPIO_SetPinState(TEST_PIN2, pinState2);
}

void Timer_3_Handler(void *arg)
{
	static bool pinState3 = false;

	if (pinState3 == false)
		pinState3 = true;
	else 
		pinState3 = false;

	CPU_GPIO_SetPinState(TEST_PIN3, pinState3);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState(TEST_PIN4, TRUE);
	CPU_GPIO_SetPinState(TEST_PIN4, FALSE);
}

void Timer_5_Handler(void *arg)
{
	CPU_GPIO_SetPinState(TEST_PIN5, TRUE);
	CPU_GPIO_SetPinState(TEST_PIN5, FALSE);
}

void Timer_6_Handler(void *arg)
{
	CPU_GPIO_SetPinState(TEST_PIN6, TRUE);
	CPU_GPIO_SetPinState(TEST_PIN6, FALSE);
}

void Timer_7_Handler(void *arg)
{
	CPU_GPIO_SetPinState(TEST_PIN7, TRUE);
	CPU_GPIO_SetPinState(TEST_PIN7, FALSE);
}


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin(TEST_PIN0, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN1, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN2, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN3, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN4, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN5, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN6, TRUE);
	CPU_GPIO_EnableOutputPin(TEST_PIN7, TRUE);
	
};

BOOL VirtualTimerTest::Level_0A()
{
	//This is for testing RTC. RTC is usually the second hardware timer.
	bool ret =  VirtTimer_SetTimer(g_VirtualTimerPerHardwareTimer+1, 0, 30000, FALSE, FALSE, Timer_1_Handler);
	return ret;
}

BOOL VirtualTimerTest::Level_0E()
{
	int periodValue = 1;
	for(int i = 0; i < 1; i++)
	{
		//periodValue = (i+1)%9 == 0 ? ((i+0)%8)+1 : (i+0)%8;
		//periodValue = (i+1)%9;
		/*if(!VirtTimer_SetTimer(0, 0, periodValue*100, FALSE, FALSE, Timer_0_Handler))
			return FALSE;*/
		//periodValue = (i+2)%9;
		/*if(!VirtTimer_SetTimer(1, 0, periodValue*1000, FALSE, FALSE, Timer_1_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(2, 0, periodValue*1000, FALSE, FALSE, Timer_2_Handler))
			return FALSE;*/
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(4, 0, periodValue*10000, FALSE, FALSE, Timer_0_Handler, RTC_32BIT))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(5, 0, periodValue*200000, FALSE, FALSE, Timer_1_Handler, RTC_32BIT))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(6, 0, periodValue*300000, FALSE, FALSE, Timer_2_Handler, RTC_32BIT))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(7, 0, periodValue*400000, FALSE, FALSE, Timer_3_Handler, RTC_32BIT))
			return FALSE;
	}


	int timerValue = 0;
	for(UINT16 i = 0; i <= 0; i++)
	{
		for (UINT16 j = 4; j <= 7; j++)
		{
			//timerValue = (i+j)%8 == 0 ? ((i+j)%8)+1 : (i+j)%8;
			VirtTimer_Start( (i+j)%8 );
		}
		//CPU_Timer_Sleep_MicroSeconds(1000000);
		/*for (UINT16 j = 0; j <= 0; j++)
		{
			VirtTimer_Stop( 0 );
		}*/
	}

	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 4)
		return Level_0E();
} //Execute

