////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GPIO.h"
#include "Parameters.h"
#include <gpio/stm32f10x_gpio.h>
#include <rcc\stm32f10x_rcc.h>
#include <fsmc\stm32f10x_fsmc.h>

GPIO::GPIO(  )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_29 | GPIO_Pin_30 | GPIO_Pin_31 ;
	//GPIO_InitStructure.GPIO_Pin = (GPIO_PIN)29;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	//GPIO_InitStructure.GPIO_Pin = (GPIO_PIN)30;
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_InitStructure.GPIO_Pin = (GPIO_PIN)31;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

BOOL GPIO::Level_0A()
{
	bool retVal3, retVal4, retVal5;

	CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN3, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN4, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN5, FALSE);

	while (true){
		/*CPU_GPIO_SetPinState(GPIO_Pin_13, TRUE);
		CPU_GPIO_SetPinState(GPIO_Pin_14, TRUE);
		CPU_GPIO_SetPinState(GPIO_Pin_15, TRUE);*/



		/*retVal3 = CPU_GPIO_GetPinState((GPIO_PIN) GPIO_J12_PIN3);
		if(retVal3)
			CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN3, TRUE);
		else
			CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN3, FALSE);*/

		retVal4 = CPU_GPIO_GetPinState((GPIO_PIN) GPIO_J12_PIN4);
		if(retVal4)
			CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN5, TRUE);
		else
			CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN5, FALSE);


		retVal5 = CPU_GPIO_GetPinState((GPIO_PIN) GPIO_J12_PIN5);
		if(retVal5)
			CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN4, TRUE);
		else
			CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN4, FALSE);



		/*retVal = CPU_GPIO_GetPinState(GPIO_Pin_13);
		retVal = CPU_GPIO_GetPinState(GPIO_Pin_14);*/
		//retVal = CPU_GPIO_GetPinState(GPIO_Pin_15);
	}
	return true;

}




BOOL GPIO::Execute( int testLevel )
{
	if (testLevel == LEVEL_0_A){
		// Indicates the start of the test
		/*CPU_GPIO_EnableOutputPin(9,TRUE);
		CPU_GPIO_SetPinState(9, TRUE);*/

		// Configure Pin 23 as the pin of interest
		/*CPU_GPIO_EnableOutputPin(23,TRUE);
		CPU_GPIO_SetPinState(23, TRUE);*/

		Level_0A();
	}
} //Execute

