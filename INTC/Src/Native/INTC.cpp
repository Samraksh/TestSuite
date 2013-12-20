////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "INTC.h"


//---//

void Timer_4_Handler( void* Param );
void Timer_3_Handler( void* Param );

INTC gIntcTestObject;

BOOL INTC::Initialize( int seedValue, int numberOfEvents )
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);

	if (!Timer_Driver :: Initialize (3, TRUE, 0, 0, Timer_3_Handler, NULL))
	{
		return FALSE;
	}
	
	if (!Timer_Driver :: Initialize (4, TRUE, 0, 0, Timer_4_Handler, NULL))
	{
		return FALSE;
	}
	
	return TRUE;

		
};

BOOL INTC::Level_0A()
{
	Timer_Driver::SetCompare( 3, 3000);
	Timer_Driver::SetCompare( 4, 3000);
	
	while(true);

	return TRUE;

}

BOOL INTC::Level_0B()
{
	Timer_Driver::SetCompare( 3, 3000);
	Timer_Driver::SetCompare( 4, 6000);
	
	while(true);
	
	return TRUE;
	
}

void Timer_3_Handler( void* Param )
{
	UINT16 randomValue = 0;
	UINT16 diffValue = 0;
	UINT16 counterValue = 0;
    

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	  {
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		CPU_GPIO_SetPinState(24,TRUE);
		CPU_GPIO_SetPinState(24,FALSE);
		
		counterValue = TIM_GetCounter(TIM3);
		Timer_Driver::SetCompare( 3, counterValue + 3000);
		    
	  }
}

void Timer_4_Handler( void* Param )
{
	UINT16 randomValue = 0;
	UINT16 diffValue = 0;
	UINT16 counterValue = 0;
    

	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	  {
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
		CPU_GPIO_SetPinState(25,TRUE);
		CPU_GPIO_SetPinState(25,FALSE);
		
		counterValue = TIM_GetCounter(TIM4);
		Timer_Driver::SetCompare( 4, counterValue + 3000);
		    
	  }
}


BOOL INTC::Execute( int testLevel )
{
	if(testLevel == 0)
		Level_0A();
	else if(testLevel == 1)
		Level_0B();
		
	return TRUE;
} //Execute

