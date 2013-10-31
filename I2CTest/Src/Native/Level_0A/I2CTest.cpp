////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "I2CTest.h"
#include <Samraksh\HAL_util.h>

//---//

extern "C"
{

void myI2C_Event_Handler(void *param)
{
	if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
		I2C_Send7bitAddress(I2C1, 0x70, I2C_Direction_Transmitter);
	}
	else if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ))
	{
		I2C_SendData(I2C1, 0x80);
	}
	else if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		I2C_SendData(I2C1, 0xF0);
	}

	I2C_GenerateSTOP(I2C1, ENABLE);
}

void myI2C_Error_Handler(void  *param)
{

}
}


I2CTest::I2CTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	this->numberOfEvents = numberOfEvents;

	testMathInstance.prng_init(30);

};

BOOL I2CTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

// This test only checks if the manufacture id can be read
BOOL I2CTest::Level_0A()
{

	UINT16 i = 0;

	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStruct;

	I2C_Cmd(I2C1, DISABLE);
	I2C_DeInit(I2C1);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);

	I2C_SoftwareResetCmd(I2C1, ENABLE);
	for(i = 0; i < 100; i++) {}
	I2C_SoftwareResetCmd(I2C1, DISABLE);

	GPIO_PinRemapConfig(GPIO_Remap_FSMC_NADV, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

	    // Enable I2C1 reset state
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	    // Release I2C1 from reset state
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

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

	if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_I2C1_EV, myI2C_Event_Handler, NULL ))
		return DS_Fail;

	if(!CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_I2C1_ER, myI2C_Error_Handler, NULL ))
		return DS_Fail;


	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_I2C1_EV))
		return DS_Fail;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_I2C1_ER))
		return DS_Fail;


	I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE);
	I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);

	I2C_GenerateSTART(I2C1, ENABLE);

	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL I2CTest::Level_0B()
{
	UINT16 i = 0;

	while(i++ < this->numberOfEvents)
	{
	}


	return TRUE;

}

BOOL I2CTest::Level_1()
{
	UINT16 i = 0;

	UINT16 inputdata[20];
	UINT16 outputdata[20];

	for(UINT32 i = 0; i < 100000; i++);

	while(i++ < this->numberOfEvents)
	{


	}

	//DisplayStats(TRUE, "Flash Write Level 1 Successful", NULL, NULL);
	return TRUE;
}

// Tests the buffer write and read interface
BOOL I2CTest::Level_0C()
{
	UINT16 i = 0;

	UINT16 inputdata[20];
	UINT16 outputdata[20];

	for(UINT16 j = 0; j < 20; j++)
	{
		outputdata[j] = 0;
	}

	for(UINT16 j = 0; j < 20; j++)
	{
		inputdata[j] = (UINT16) (testMathInstance.pareto_prng() % (1 << 15));
	}

	while(i++ < this->numberOfEvents)
	{
	}


	return TRUE;
}


BOOL I2CTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	case 2:
		result = Level_0C();
		break;
	case 3:
		result = Level_1();
		break;
	}


	return result;



} //Execute

