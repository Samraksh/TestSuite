////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Timers.h"
#include <tim\netmf_timers.h>

//---//

int testCount;

testMath g_testMathInstance;
bool Level_1_A_Test = false;
bool Level_1_B_Test = false;
bool Level_2_Test = false;
bool Level_0_A_Test = false;
bool Level_0_B_Test = false;

UINT16 randomBuffer[600];
UINT16 bufferIterator = 0;

void ISR_TIMER( void* Param );


Timers::Timers( int seedValue, int numberOfEvents )
{
	this->seedValue = seedValue;
	this->numberOfEvents = numberOfEvents;
	g_testMathInstance.prng_init(seedValue);
	//this->testMathInstance.pareto_prng()_init(seedValue);
	// Initializes the gpio pins
	CPU_GPIO_Initialize();
	CPU_INTC_Initialize();
};

BOOL Timers::testMathLibrary()
{

}

BOOL Timers::Level_0A(int Timer)
{
	Level_0_A_Test = true;
	Timer = 3;
	// Initialize the timer in free running mode with ISR_TIMER_4 as interrupt handler
	if (!Timer_Driver :: Initialize (3, TRUE, 0, 0, ISR_TIMER, NULL))
	{
		return FALSE;
	}

	TIM_SetCounter(TIM3,0);
	Timer_Driver::SetCompare( 3, 5520);
	testCount = 0;
	
	while (testCount < 50){
		CPU_GPIO_SetPinState(22,FALSE);
	}
	
	Timer_Driver::Uninitialize(3);

	Level_0_A_Test = false;
	return TRUE;

}

BOOL Timers::Level_0B()
{
}

BOOL Timers::Level_1A()
{


}

BOOL Timers::Level_1B(int Timer)
{

}

BOOL Timers::Level_2()
{


}



BOOL Timers::Execute( int testLevel )
{
	CPU_GPIO_SetPinState(0,TRUE);
	Timers::Level_0A(3);
	CPU_GPIO_SetPinState(0,FALSE);
			
} //Execute

void ISR_TIMER( void* Param )
{
	UINT16 randomValue = 0;
	UINT16 diffValue = 0;
	UINT16 counterValue = 0;
    

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	  {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		CPU_GPIO_SetPinState(22,TRUE);
		testCount++;
		//TIM_SetCounter(TIM3,0);
		//Timer_Driver::SetCompare( 3, 5520);
		counterValue = TIM_GetCounter(TIM3);
		Timer_Driver::SetCompare( 3, counterValue + 5520);
		    
	  }
}


