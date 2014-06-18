////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
//TODO: AnanthAtSamraksh -- below 2 includes need to be fixed
#include "D:/AnanthAtSamraksh/MF/MicroFrameworkPK_v4_3/DeviceCode/Include/Samraksh/VirtualTimer.h"
#include "D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\Include\Time_decl.h"


//---//

//Staring a child timer from timer 1 causes both timers to be off (irrespective of starting first or second; with or without a starting delay)
void Timer_0_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);
	//CPU_Timer_Sleep_MicroSeconds(100);
	//VirtTimer_Start(1);
	////VirtTimer_Start(3);
	////VirtTimer_Start(4);

	/*static int handlerCount = 0;
	handlerCount++;

	if ((handlerCount%11)==0){
		//VirtTimer_Start(2);
	}
	else if ((handlerCount % 9)==0){
		VirtTimer_Start(3);
	}
	else if ((handlerCount % 10)==0){
		VirtTimer_Start(4);
	}*/
	/*if ((handlerCount%100)==0){
		CPU_Timer_Sleep_MicroSeconds(1000000);
	}*/
}

//Staring a child timer from timer 2 causes child timer to be off (irrespective of starting first or second; with or without a starting delay)
void Timer_1_Handler(void *arg)
{
	//GLOBAL_LOCK(irq);

	CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
	//HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1000000);
	//for(volatile UINT32 i = 0; i < 100000; i++);
	CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);
	//CPU_Timer_Sleep_MicroSeconds(100);
	////VirtTimer_Start(3);

	//ENABLE_INTERRUPTS();
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 55, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 55, FALSE);
	//CPU_Timer_Sleep_MicroSeconds(1000);
	//VirtTimer_Start(3);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 58, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 58, FALSE);
	//CPU_Timer_Sleep_MicroSeconds(1000);
	//VirtTimer_Start(4);
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
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 52, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 55, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 58, TRUE);
	/*CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);*/

	//Tasklet_Initialize();

	//if(!VirtTimer_Initialize())
		//hal_printf("Error");
};

BOOL VirtualTimerTest::Level_0A()
{
	VirtTimer_SetTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);

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
		if(!VirtTimer_SetTimer(0, 0, periodValue*10000, FALSE, FALSE, Timer_0_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(1, 0, periodValue*1000, FALSE, FALSE, Timer_1_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(2, 0, periodValue*1000, FALSE, FALSE, Timer_2_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(3, 0, periodValue*1000, FALSE, FALSE, Timer_3_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		/*if(!VirtTimer_SetTimer(4, 0, periodValue*1000, FALSE, FALSE, Timer_4_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(5, 0, periodValue*1000, FALSE, FALSE, Timer_5_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(6, 0, periodValue*1000, FALSE, FALSE, Timer_6_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(7, 0, periodValue*1000, FALSE, FALSE, Timer_7_Handler))
			return FALSE;*/
	}

	//CPU_Timer_Sleep_MicroSeconds(1000000);

	for (UINT16 j = 0; j <= 3; j++)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);
		CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);
		CPU_GPIO_SetPinState((GPIO_PIN) 55, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 55, FALSE);
		CPU_GPIO_SetPinState((GPIO_PIN) 58, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 58, FALSE);
	}



	int timerValue = 0;
	for(UINT16 i = 0; i < 1; i++)
	{
		for (UINT16 j = 0; j <= 3; j++)
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





	/*VirtTimer_Start(7);
	VirtTimer_Start(6);
	VirtTimer_Start(5);
	VirtTimer_Start(2);
	VirtTimer_Start(3);
	VirtTimer_Start(4);
	VirtTimer_Start(1);*/



	/*if(!VirtTimer_Start(1))
		return FALSE;
	if(!VirtTimer_Start(2))
		return FALSE;
	if(!VirtTimer_Start(3))
		return FALSE;
	if(!VirtTimer_Start(4))
		return FALSE;*/

	return TRUE;
}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
} //Execute

