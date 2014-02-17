////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SPI.h"


// This tests the SPI interface. The simplest way to verify that this works is by attaching a logic ananlyzer and checking the output. //

SPI_CONFIGURATION config;
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStruct;

SPI::SPI()
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	//this->numberOfEvents = numberOfEvents;
	//testMathInstance.prng_init(seedValue);

	// Stores the configuration of the spi
	config.DeviceCS			= 10;
	config.CS_Active		= false;
	config.CS_Setup_uSecs	= 0;
	config.CS_Hold_uSecs	= 0;
	config.MSK_IDLE			= false;
	config.MSK_SampleEdge	= false;
	config.Clock_RateKHz	= 2;
	config.MD_16bits		= false;
	config.SPI_mod			= SPIBUS2;		//Needs to change based on type of radio (LR vs regular)

	SPI_StructInit(config);
	GPIO_Config(config);

#if 0
	//Taken from D:\AnanthAtSamraksh\APPS-FOR-TESTING-MOTE\ExtRad_test\src\main.c
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//Configure SPI2 mode
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO, ENABLE);
	//Enable the functions below
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	// SPI clock and MOSI pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //(AF -??; PP - Push Pull --- for MOSI)
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// SPI MISO pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //(MISO should be floating)
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;		//Pull the pin low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;	//Falling or raising edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_SSOutputCmd(SPI2, DISABLE);
	SPI_Cmd(SPI2, ENABLE);
#endif
};

BOOL SPI::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

/* This test toggles a GPIO pin high and then low */
BOOL SPI::Level_0A()
{
	//CPU_GPIO_Uninitialize();
	//CPU_SPI_Initialize();

	//CPU_SPI_ENABLE(config);


	//SPI_StructInit(config);
	//GPIO_Config(config);

	CPU_SPI_Xaction_Start(config);
	int data = 1;
	CPU_SPI_WriteByte(config, data);
	CPU_SPI_Xaction_Stop(config);

	for(int i = 0; i < 100000; ++i);
	for(int i = 0; i < 100000; ++i);

	//SPI_StructInit(config);
	//GPIO_Config(config);

	CPU_SPI_Xaction_Start(config);
	data = 0;
	CPU_SPI_WriteByte(config, data);
	CPU_SPI_Xaction_Stop(config);

	//CPU_GPIO_Uninitialize();
	//CPU_SPI_Uninitialize();

	return TRUE;
}

BOOL SPI::Level_0B()
{
}




BOOL SPI::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		while(true)
		{
			result = Level_0A();
		}
		break;
	case 1:
		result = Level_0B();
		break;
	default:
		break;
	}

} //Execute

