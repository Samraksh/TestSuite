////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LCD.h"
#include "Parameters.h"
#include <i2c/stm32f10x_i2c.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>

const uint8_t LCD_NUM[63] = { 0x00, 0xee, 0xef, 0x8d, 0xed, 0x8f, 0x8e, 0xcd, 0x6e, 0x60, 0x65, 0x6e, 0x0d, 0xc6, 0xec, 0xed, 0xae, 0xea, 0x8c, 0xcb, 0xe0, 0x6d, 0x29, 0xc5, 0x6e, 0x6a, 0xa7, 0xe7, 0x4f, 0x07, 0x67, 0xaf, 0x8e, 0xeb, 0x4e, 0x40, 0x41, 0x2f, 0x60, 0x8f, 0x46, 0x47, 0xae, 0xea, 0x06, 0xcb, 0x0f, 0x45, 0x45, 0xe3, 0x6e, 0x6b, 0xa7, 0xed, 0x60, 0xa7, 0xe3, 0x6a, 0xcb, 0xcf, 0xe0, 0xef, 0xeb };

LCD::LCD(  )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStruct;

	GPIO_PinRemapConfig(GPIO_Remap_FSMC_NADV, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStruct.I2C_ClockSpeed = 100000;
  	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStruct.I2C_OwnAddress1 = 0x33;

  	I2C_Cmd(I2C1, ENABLE);
  	I2C_Init(I2C1, &I2C_InitStruct);  
};

BOOL LCD::Level_0A()
{
	
I2C_GenerateSTART(I2C1, ENABLE);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) { ; }

  I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) { ; }

  I2C_SendData(I2C1, 0x80); // Load-data-pointer, reset to 0, 10000000
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  I2C_SendData(I2C1, 0x48); // Mode Set, 01001000
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  // Start sending display bytes
  I2C_SendData(I2C1, LCD_NUM[LCD_CHAR_1]);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  I2C_SendData(I2C1, LCD_NUM[LCD_CHAR_2]);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  I2C_SendData(I2C1, LCD_NUM[LCD_CHAR_3]);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }

  I2C_SendData(I2C1, LCD_NUM[LCD_CHAR_4]);
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) { ; }
}


BOOL LCD::Execute( int testLevel )
{
	CPU_GPIO_SetPinState(0,TRUE);
	LCD::Level_0A();
	CPU_GPIO_SetPinState(0,FALSE);
} 
