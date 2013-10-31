////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "ADC.h"
#include <Samraksh\HAL_util.h>

//---//

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

volatile UINT16 Index = 0;

volatile UINT16 ADC_RegularConvertedValueTab[1000], ADC_InjectedConvertedValueTab[32];
volatile UINT16 ADCConvertedValue;

extern "C"
{
extern DeviceStatus ADC_ConfigureContinuousMode(UINT16* sampleBuff1, UINT32 numSamples, UINT32 samplingTime, HAL_CALLBACK_FPN userCallback, void* Param);
}

ADCTest::ADCTest( int seedValue, int numberOfEvents )
{
	//CPU_GPIO_Initialize();

	this->numberOfEvents = numberOfEvents;

	//testMathInstance.prng_init(30);

};

BOOL ADCTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

void RCC_Configuration()
{
	 RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	  /* Enable peripheral clocks ------------------------------------------------*/
	  /* Enable DMA1 clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	  /* Enable GPIOA, GPIOC, ADC1 and TIM1 clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB |
	                         RCC_APB2Periph_ADC1, ENABLE);



}

void GPIO_Configuration()
{
	 GPIO_InitTypeDef GPIO_InitStructure;

	  /* Configure TIM1_CH1 (PA8) as alternate function push-pull */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /* Configure PC.06 as output push-pull */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  /* Configure PC.01 and PC.04 (ADC Channel11 and Channel14) as analog input */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
}


extern "C"
{
	void ADC_HANDLER(void *param)
	{
		  /* Clear ADC1 JEOC pending interrupt bit */
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

		CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);

	}

	void DMA_HANDLER(void *param)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET)
		{
			DMA_ClearITPendingBit(DMA1_IT_TC1);

			CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
						CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		}
	}

	void TIM_HANDLER(void *param)
	{
		if(TIM_GetFlagStatus(TIM4, TIM_IT_CC4))
		{

			TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);


		}
	}
}

// This test only checks if the manufacture id can be read
BOOL ADCTest::Level_0A()
{


	ADC_InitTypeDef           ADC_InitStructure;
	DMA_InitTypeDef           DMA_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);

	 /* System clocks configuration ---------------------------------------------*/
	  RCC_Configuration();

	  /* GPIO configuration ------------------------------------------------------*/
	  GPIO_Configuration();

	  // Active timer 1 cc interrupt
	 	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1, DMA_HANDLER, NULL) )
	 		return FALSE;


	 	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_ADC1_2, ADC_HANDLER, NULL) )
		 		return FALSE;

	 	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM4, TIM_HANDLER, NULL) )
	 			 		return FALSE;

#if 0
	  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	  TIM_TimeBaseStructure.TIM_Period = 20000;
	  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	 	  /* TIM1 channel1 configuration in PWM mode */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = 0x01;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
#endif

	    UINT16 prescalerValue = 0;


	    UINT16 frequency =  CPU_MicrosecondsToTicks((UINT32) 1000) * 2 ;

	  /* Time Base configuration */
	    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	    TIM_TimeBaseStructure.TIM_Period = frequency;
	    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	    /* TIM1 channel1 configuration in PWM mode */
	    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	    TIM_OCInitStructure.TIM_Pulse = (frequency/2);
	    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	    TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	    //TIM_SetCompare4(TIM4, 20000);

	    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	    TIM_ARRPreloadConfig(TIM4, ENABLE);

	    //TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);



	 /* DMA1 channel1 configuration ----------------------------------------------*/
	  DMA_DeInit(DMA1_Channel1);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_RegularConvertedValueTab;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	  DMA_InitStructure.DMA_BufferSize = 1000;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	  /* Enable DMA1 channel1 */
	  DMA_Cmd(DMA1_Channel1, ENABLE);

	  /* ADC1 configuration ------------------------------------------------------*/
	  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_CC4;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfChannel = 1;
	  ADC_Init(ADC1, &ADC_InitStructure);

	  //ADC1->CR2 = 5 << 17;
	  //ADC1->CR2 = 1 << 20;



	  /* ADC1 regular channel14 configuration */
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);



	  /* Enable ADC1 DMA */
	  ADC_DMACmd(ADC1, ENABLE);

	  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

	  /* Enable ADC1 */
	  //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	  ADC_Cmd(ADC1, ENABLE);

	  /* Enable ADC1 reset calibration register */
	  ADC_ResetCalibration(ADC1);
	  /* Check the end of ADC1 reset calibration register */
	  while(ADC_GetResetCalibrationStatus(ADC1));

	  /* Start ADC1 calibration */
	  ADC_StartCalibration(ADC1);
	  /* Check the end of ADC1 calibration */
	  while(ADC_GetCalibrationStatus(ADC1));

	  /* Start ADC1 Software Conversion */
	  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	  /* TIM1 counter enable */

	  /* TIM1 counter enable */
	  TIM_Cmd(TIM4, ENABLE);
	       /* TIM1 main Output Enable */
	       //TIM_CtrlPWMOutputs(TIM1, ENABLE);

	  //TIM_CtrlPWMOutputs(TIM1, ENABLE);


	  while(TRUE);


	  /*!< At this stage the microcontroller clock setting is already configured,
	        this is done through SystemInit() function which is called from startup
	        file (startup_stm32f10x_xx.s) before to branch to application main.
	        To reconfigure the default setting of SystemInit() function, refer to
	        system_stm32f10x.c file
	      */
#if 0
	   /* System clocks configuration ---------------------------------------------*/
	   RCC_Configuration();


	   /* GPIO configuration ------------------------------------------------------*/
	   GPIO_Configuration();

		if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_DMA_CHANNEL1, DMA_HANDLER, NULL) )
		 		return FALSE;

	 	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_ADC1_2, ADC_HANDLER, NULL) )
		 		return FALSE;

	 	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM4, TIM_HANDLER, NULL) )
		 		return FALSE;


	   /* TIM1 configuration ------------------------------------------------------*/
	   /* Time Base configuration */
	   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	   TIM_TimeBaseStructure.TIM_Period = 20000;
	   TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
	   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	   TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	   /* TIM1 channel1 configuration in PWM mode */
	   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	   TIM_OCInitStructure.TIM_Pulse = 0x7F;
	   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	   TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	   /* DMA1 Channel1 Configuration ----------------------------------------------*/
	   DMA_DeInit(DMA1_Channel1);
	   DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_RegularConvertedValueTab;
	   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	   DMA_InitStructure.DMA_BufferSize = 32;
	   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	   DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	   DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	   DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	   /* Enable DMA1 channel1 */
	   DMA_Cmd(DMA1_Channel1, ENABLE);



	   /* ADC1 configuration ------------------------------------------------------*/
	   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	   ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	   ADC_InitStructure.ADC_NbrOfChannel = 1;
	   ADC_Init(ADC1, &ADC_InitStructure);

	   /* ADC1 regular channel14 configuration */
	   ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_13Cycles5);

	   /* Set injected sequencer length */
	   ADC_InjectedSequencerLengthConfig(ADC1, 1);
	   /* ADC1 injected channel Configuration */
	   ADC_InjectedChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_71Cycles5);
	   /* ADC1 injected external trigger configuration */
	   ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

	   /* Enable automatic injected conversion start after regular one */
	   ADC_AutoInjectedConvCmd(ADC1, ENABLE);

	   /* Enable ADC1 DMA */
	   ADC_DMACmd(ADC1, ENABLE);

	   /* Enable ADC1 external trigger */
	   ADC_ExternalTrigConvCmd(ADC1, ENABLE);

	   /* Enable JEOC interrupt */
	   ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);

	   /* Enable ADC1 */
	   ADC_Cmd(ADC1, ENABLE);

	   /* Enable ADC1 reset calibration register */
	   ADC_ResetCalibration(ADC1);
	   /* Check the end of ADC1 reset calibration register */
	   while(ADC_GetResetCalibrationStatus(ADC1));

	   /* Start ADC1 calibration */
	   ADC_StartCalibration(ADC1);
	   /* Check the end of ADC1 calibration */
	   while(ADC_GetCalibrationStatus(ADC1));

	   /* TIM1 counter enable */
	   TIM_Cmd(TIM4, ENABLE);
	   /* TIM1 main Output Enable */
	   //TIM_CtrlPWMOutputs(TIM1, ENABLE);
#endif
	return TRUE;
}

extern "C"
{
void ADCUserHandler(void *Param)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}
}

UINT16 adcBuffer[1000];

// This test writes data and then verifies the write has been successful
BOOL ADCTest::Level_0B()
{



	AD_ConfigureContinuousMode(adcBuffer, 1000, 1000, ADCUserHandler, NULL);

	while(TRUE);


	return TRUE;
}

BOOL ADCTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL ADCTest::Level_0C()
{
	return TRUE;
}


// Tests the buffer write and read interface for various address values
BOOL ADCTest::Level_0D()
{
	return TRUE;
}


BOOL ADCTest::Execute( int testLevel )
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
		result = Level_0D();
		break;
	case 4:
		result = Level_1();
		break;
	}


	return result;



} //Execute

