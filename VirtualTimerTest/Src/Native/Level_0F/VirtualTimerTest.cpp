////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "VirtualTimerTest.h"
#include "../DeviceCode/Include/Samraksh/VirtualTimer.h"
#include "../DeviceCode/Include/Time_decl.h"


//---//

UINT32 VirtualTimerTest::prevTicks = 0;

void Timer_0_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void Timer_1_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
}

void Timer_2_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
}

void Timer_3_Handler(void *arg)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
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


VirtualTimerTest::VirtualTimerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);
};

BOOL VirtualTimerTest::Level_0A()
{
	VirtTimer_SetTimer(1, 0, 30000, FALSE, FALSE, Timer_1_Handler);

	return TRUE;
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
		if(!VirtTimer_SetTimer(3, 0, periodValue*500000, FALSE, FALSE, Timer_3_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(4, 0, periodValue*500000, FALSE, FALSE, Timer_4_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(5, 0, periodValue*500000, FALSE, FALSE, Timer_5_Handler))
			return FALSE;
		//periodValue = (i+1)%9;
		if(!VirtTimer_SetTimer(6, 0, periodValue*500000, FALSE, FALSE, Timer_6_Handler))
			return FALSE;
		//periodValue = (i+2)%9;
		if(!VirtTimer_SetTimer(7, 0, periodValue*500000, FALSE, FALSE, Timer_7_Handler))
			return FALSE;
	}


	int timerValue = 0;
	for(UINT16 i = 0; i <= 0; i++)
	{
		for (UINT16 j = 3; j <= 7; j++)
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


BOOL VirtualTimerTest::Level_0F()
{
	UINT32 i = 0; currentTicks = 0;

	while(currentTicks >= 0 && currentTicks < ((1<<32)-1))
	{
		currentTicks = VirtTimer_GetTicks(VIRT_TIMER_TIME);

		if(currentTicks < 0 || currentTicks < prevTicks)
		{
			if(prevTicks > ((1<<31)-1))
			{
				hal_printf("count: %u currentTicks > prevTicks \r\n", i);
				hal_printf("currentTicks: %u\r\n", currentTicks);
				break;
			}
			else
			{
				//Timer_0_Handler(NULL);
				DisplayStats(false, "ERROR: currentTicks is less than prevTicks", NULL, 0);
				return false;
			}
		}
		else
		{
			//Timer_1_Handler(NULL);
			if(!(i % 10000000))
			{
				hal_printf("count: %llu currentTicks > prevTicks \r\n", i);
				hal_printf("currentTicks: %u\r\n", currentTicks);
			}
		}
		prevTicks = currentTicks;
		i++;
	}

	DisplayStats(true, "SUCCESS: currentTicks is always greater than prevTicks", NULL, 0);

}


BOOL VirtualTimerTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 5)
		return Level_0F();
} //Execute

