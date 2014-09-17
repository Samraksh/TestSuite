////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* Test to check if time increases monotonically or not. */
#include "TimeTest.h"
#include "..\DeviceCode\Include\Time_decl.h"


//---//

UINT64 TimeTest::prevTicks = 0;

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

BOOL TimeTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

TimeTest::TimeTest( int seedValue, int numberOfEvents )
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


BOOL TimeTest::Level_0A()
{
	INT64 prevTime = 0, currentTime = 0;

	while(true)
	{
		prevTime = HAL_Time_CurrentTime();
		currentTime = HAL_Time_CurrentTime();

		if(currentTime > prevTime)
			Timer_0_Handler(NULL);
		else
		{
			Timer_1_Handler(NULL);
			break;
		}
	}

	return TRUE;
}


//Under the current implementation, HAL_Time_CurrentTime and HAL_Time_CurrentTicks return the same value (ticks) for the "AdvancedTimer" driver
BOOL TimeTest::Level_0B()
{
	UINT64 currentTicks = 0;
	UINT64 i = 0;

	//The compareValue can be any power of 2 value above 2^32. It has been set to 2^34 below.
	UINT64 compareValue = (0x3FFFFFFFF);

	while(currentTicks >= 0 && currentTicks < (compareValue-1))
	{
		//prevTicks = HAL_Time_CurrentTicks();
		////HAL_Time_Sleep_MicroSeconds(5000);
		currentTicks = HAL_Time_CurrentTicks();

		if( currentTicks > ( compareValue-1 ) )
		{
			hal_printf("currentTicks: %llu; prevTicks: %llu\r\n", currentTicks, prevTicks);
			break;
		}

		if(currentTicks < 0 || currentTicks < prevTicks)
		{
			//Sometimes, bigCounter is updated after ticks flow-over, which causes this test to fail.
			//That is why, the ticks are obtained once again after waiting for sometime.
			/*HAL_Time_Sleep_MicroSeconds(5000);
			currentTicks = HAL_Time_CurrentTicks();
			if(currentTicks < 0 || currentTicks < prevTicks)
			{*/
				//Timer_1_Handler(NULL);
				hal_printf("count: %llu currentTicks < prevTicks \r\n", i);
				hal_printf("currentTicks: %llu; prevTicks: %llu\r\n", currentTicks, prevTicks);
				DisplayStats(false, "ERROR: currentTicks is less than prevTicks", NULL, 0);
				//break;
			/*}
			else
			{
				hal_printf("count: %llu currentTicks > prevTicks \r\n", i);
				hal_printf("currentTicks: %llu; prevTicks: %llu\r\n", currentTicks, prevTicks);
			}*/
		}
		else
		{
			//Timer_0_Handler(NULL);
			if(!(i % 1000000))
			{
				hal_printf("count: %llu currentTicks > prevTicks \r\n", i);
				hal_printf("currentTicks: %llu; prevTicks: %llu\r\n", currentTicks, prevTicks);
			}
		}
		i++;

		prevTicks = currentTicks;
	}
	DisplayStats(true, "SUCCESS: currentTicks is always greater than prevTicks", NULL, 0);

	return TRUE;
}


BOOL TimeTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();

} //Execute

