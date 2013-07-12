#include "Timer.h"

int testCount = 0;
int secCount=0;


TimerTest::TimerTest( UINT32 DisplayIntervalSeconds, UINT32 TimerDurationSeconds )
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
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);

	//Timer_Driver :: Initialize (2 , TRUE, 0, 0, ISR_TIMER, NULL);

	//Timer_Driver :: SetCompare(2 , 1000);

	m_lastRead = 0;

	m_toggleFlag= FALSE;

};

// Test the unsigned int theory
BOOL TimerTest::Level_0B()
{

	while(TRUE)
	{
		UINT64 lastVal = m_lastRead;

		UINT16 lastSixteenBit = m_lastRead & 0x0000FFFFull;

		UINT16 value = Timer_Driver :: GetCounter (Timer_Driver :: c_SystemTimer);

		m_lastRead &= (0xFFFF0000ull);

		if(Timer_Driver :: DidTimeOverFlow( Timer_Driver::c_SystemTimer ) || (value < lastSixteenBit))
		{

			CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, TRUE);
			CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, FALSE);

			Timer_Driver :: ClearTimeOverFlow( Timer_Driver::c_SystemTimer );
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

BOOL TimerTest::Level_0C()
{
	UINT8 i = 0;

	INT64 localTime  = 0;

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
	    CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
	}


	while(true)
	{
		localTime = Time_GetLocalTime();

		if(localTime > m_lastRead)
		{
				CPU_GPIO_SetPinState((GPIO_PIN) 24, m_toggleFlag);
				m_toggleFlag = ~m_toggleFlag;

		}
		else
		{
				CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
				CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);
		}

				m_lastRead = localTime;
	}

	return TRUE;
}

// HAL_Time_CurrentTicks test
BOOL TimerTest::Level_0A()
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
	    CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
	}


	while(true)
	{

		UINT64 curTicks = HAL_Time_CurrentTicks();



		if(curTicks > m_lastRead)
		{

			CPU_GPIO_SetPinState((GPIO_PIN) 24, m_toggleFlag);
			m_toggleFlag = ~m_toggleFlag;

		}
		else
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 1, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 1, FALSE);
		}

		m_lastRead = curTicks;
	}

        return TRUE;

}

void TimerTest :: ISR( void* Param )
{
}

BOOL TimerTest::Execute(int level )
{
	if(level == 0)
		return Level_0A();
	else
		return Level_0B();

}; //Execute


//--//


