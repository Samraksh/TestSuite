#include "GlobalLock.h"

int testCount = 0;
int secCount=0;


GlobalLockTest::GlobalLockTest( UINT32 DisplayIntervalSeconds, UINT32 GlobalLockDurationSeconds )
{
	// Initializes the gpio pins
	//CPU_GPIO_Initialize();
	//CPU_INTC_Initialize();
	//HAL_Time_Initialize();

	//Time_Initialize();

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, FALSE);

	//GlobalLock_Driver :: Initialize (2 , TRUE, 0, 0, ISR_GlobalLock, NULL);

	//GlobalLock_Driver :: SetCompare(2 , 1000);

	m_lastRead = 0;

	m_toggleFlag= FALSE;

};

BOOL GlobalLockTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

// Test the unsigned int theory
BOOL GlobalLockTest::Level_0B()
{

	while(TRUE)
	{
		UINT64 lastVal = m_lastRead;

		UINT16 lastSixteenBit = m_lastRead & 0x0000FFFFull;

		UINT16 value = Timer_Driver :: GetCounter (Timer_Driver :: c_SystemTimer);

		m_lastRead &= (0xFFFF0000ull);

		if(Timer_Driver :: DidTimeOverFlow( Timer_Driver :: c_SystemTimer ) || (value < lastSixteenBit))
		{

			CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, TRUE);
			CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, FALSE);

			Timer_Driver :: ClearTimeOverFlow( Timer_Driver :: c_SystemTimer );
    	//STM_EVAL_LEDToggle((Led_TypeDef)0); //Green
			m_lastRead += (0x1ull << 16);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		if(m_lastRead < lastVal)
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);
		}
		else
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 24, m_toggleFlag);
			m_toggleFlag = ~m_toggleFlag;
		}

	}

	return TRUE;

}

BOOL GlobalLockTest::Level_0A()
{
	UINT8 i = 0;

	while(i++ < 10)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
		CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);
		CPU_GPIO_SetPinState((GPIO_PIN) 3, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 3, FALSE);
		CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
	    CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
	    CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	    CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	}


DisplayStats(FALSE, "ERROR:Test is an endless loop", NULL, 0);
	while(TRUE)
	{

		i = 0;

		{


			while(i++ < 1000)
			{
				GLOBAL_LOCK(irq);
				CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
				CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);

			}
		}


		i = 0;

		while(i++ < 1000)
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);

		}



	}

        return TRUE;

}

void GlobalLockTest :: ISR( void* Param )
{
}

BOOL GlobalLockTest::Execute(int level )
{
	if(level == 0)
		return Level_0A();
	else
		return Level_0B();

}; //Execute


//--//


