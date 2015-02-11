////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"
#include "../DeviceCode/Include/Time_decl.h"


//---//

void Timer_0_Handler(void *arg)
{
	static bool pinState0 = false;

	if (pinState0 == false)
		pinState0 = true;
	else 
		pinState0 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 24, pinState0);
}

void Timer_1_Handler(void *arg)
{
	static bool pinState1 = false;

	if (pinState1 == false)
		pinState1 = true;
	else 
		pinState1 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, pinState1);
}

void Timer_2_Handler(void *arg)
{
	static bool pinState2 = false;

	if (pinState2 == false)
		pinState2 = true;
	else 
		pinState2 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 29, pinState2);
}

void Timer_3_Handler(void *arg)
{
	static bool pinState3 = false;

	if (pinState3 == false)
		pinState3 = true;
	else 
		pinState3 = false;

	CPU_GPIO_SetPinState((GPIO_PIN) 30, pinState3);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 31, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 31, FALSE);
}

void Timer_5_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 2, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 2, FALSE);
}

void Timer_6_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 4, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 4, FALSE);
}

void Timer_7_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
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
	VirtTimer_SetTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);

	return TRUE;
}

BOOL VirtualTimerTest::Level_0E()
{
	UINT64 readTicks = 0;
	UINT64 prevTicks = 0;

	readTicks =	VirtTimer_GetTicks(0);
	prevTicks = readTicks;
	while (true){
		readTicks =	VirtTimer_GetTicks(0);
		if (readTicks < prevTicks){
			hal_printf("< ct: %lld pt: %lld\r\n",readTicks, prevTicks);
		}
		if (readTicks > (prevTicks + 10000000) ){
			hal_printf("> ct: %lld pt: %lld\r\n",readTicks, prevTicks);
		}
		prevTicks = readTicks;
		CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	}
	
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
		if(!VirtTimer_SetTimer(3, 0, periodValue*100000, FALSE, FALSE, Timer_0_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(4, 0, periodValue*200000, FALSE, FALSE, Timer_1_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(5, 0, periodValue*300000, FALSE, FALSE, Timer_2_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(6, 0, periodValue*400000, FALSE, FALSE, Timer_3_Handler))
			return FALSE;
	}


	int timerValue = 0;
	for(UINT16 i = 0; i <= 0; i++)
	{
		for (UINT16 j = 3; j <= 6; j++)
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

