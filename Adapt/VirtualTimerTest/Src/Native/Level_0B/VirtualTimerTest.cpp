////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"
#include "../DeviceCode/Include/Time_decl.h"


//---//
void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 51, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 51, FALSE);
}

void Timer_4_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);
}

void Timer_5_Handler(void *arg)
{
	//GLOBAL_LOCK(irq);

	CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);

	//ENABLE_INTERRUPTS();
}

void Timer_6_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 55, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 55, FALSE);
}

void Timer_7_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 58, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 58, FALSE);
}

void Timer_8_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 58, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 58, FALSE);
}

VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 51, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 52, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 55, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 58, TRUE);
};

BOOL VirtualTimerTest::Level_0A()
{
	VirtTimer_SetTimer(3, 0, 30000, FALSE, FALSE, Timer_3_Handler);

	return TRUE;
}

// Creates two timers
BOOL VirtualTimerTest::Level_0B()
{
	//AnanthAtSamraksh - only 2 virtTimers work simultaneously. In order for more timers to work, the rest are made
	//one-shot and invoked from the other timers.
	//AnanthAtSamraksh -- an odd behavior with the virtTimer. The virtTimers have to be started in the reverse order in which they are initialized.
	//The timer that is set first and started last, seems to get most of the attention of the scheduler and hence has the lowest delay.
	//If the startTime and period are changed, only 4 timers work simultaneously.
	//With the same startTime and period (1ms), only 5 timers work simultaneously.

	int periodValue = 1;
	for(int i = 0; i < 1; i++)
	{
		//periodValue = (i+1)%9 == 0 ? ((i+0)%8)+1 : (i+0)%8;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(3, 0, periodValue*10000, FALSE, FALSE, Timer_3_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(4, 0, periodValue*1000, FALSE, FALSE, Timer_4_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(5, 0, periodValue*1000, FALSE, FALSE, Timer_5_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(6, 0, periodValue*1000, FALSE, FALSE, Timer_6_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(7, 0, periodValue*1000, FALSE, FALSE, Timer_7_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		/*if(!VirtTimer_SetTimer(5, 0, periodValue*1000, FALSE, FALSE, Timer_5_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(6, 0, periodValue*1000, FALSE, FALSE, Timer_6_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(7, 0, periodValue*1000, FALSE, FALSE, Timer_7_Handler))
			return FALSE;*/
	}

	//CPU_Timer_Sleep_MicroSeconds(1000000);

	int timerValue = 0;
	for(UINT16 i = 0; i < 1; i++)
	{
		for (UINT16 j = 3; j < 8; j++)
		{
			//if((i+j)%5 == 1 || (i+j)%5 == 2)
				//hal_printf("");
			//timerValue = (i+j)%8 == 0 ? ((i+j)%8)+1 : (i+j)%8;
			VirtTimer_Start( (i+j)%8 );
			//CPU_Timer_Sleep_MicroSeconds(1000);

			//hal_printf("%llu \n", VirtTimer_GetTicks(1));
			//hal_printf("%llu\n", HAL_Time_CurrentTicks(1));
			//hal_printf("%llu\n", HAL_Time_CurrentTicks(2));

			////UINT16 timer = (i+j)%8;
			////hal_printf("Time in ticks is %llu\n", VirtTimer_GetTicks(timer));
		}
		//CPU_Timer_Sleep_MicroSeconds(10000000);
		/*for (int j = 1; j <= 8; j++)
		{
			timerValue = (i+j)%8 == 0 ? ((i+j)%8)+1 : (i+j)%8;
			VirtTimer_Stop( timerValue );
		}*/
	}


	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
} //Execute

