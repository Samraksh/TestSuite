////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GPIO.h"
#include "Parameters.h"
#include <i2c/stm32f10x_i2c.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>

GPIO::GPIO(  )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

BOOL GPIO::Level_0A()
{
	int usThreadSleep = (int)( (1/frequency)/2 * 1000000);
	while (true){
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET);

		GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_SET);

  		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);
		HAL_Time_Sleep_MicroSeconds(usThreadSleep);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);
  		GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);

		GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
  		GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);

  		GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);
		HAL_Time_Sleep_MicroSeconds(usThreadSleep);
	}
	return TRUE;

}

BOOL GPIO::Level_0B()
{
}




BOOL GPIO::Execute( int testLevel )
{
	if (testLevel == LEVEL_0_A){
		// Indicates the start of the test
		CPU_GPIO_EnableOutputPin(9,TRUE);
		CPU_GPIO_SetPinState(9, TRUE);

		// Configure Pin 23 as the pin of interest
		CPU_GPIO_EnableOutputPin(23,TRUE);
		CPU_GPIO_SetPinState(23, TRUE);

		Level_0A();
	}
} //Execute

