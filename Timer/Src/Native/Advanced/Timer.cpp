#include "Timer.h"

int testCount = 0;
int secCount=0;

extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;

static void __SVC()  { __asm volatile ("svc #0x32");}

extern "C"
{
	void ISR_TIM1( void* Param );
	void ISR_TIM2( void* Param );
}

UINT32 randomValueCounter = 0;
UINT32   compareArray[1000];
BOOL testComplete = FALSE;

TimerTest::TimerTest( UINT32 DisplayIntervalSeconds, UINT32 TimerDurationSeconds )
{
	// Initializes the gpio pins
	//CPU_GPIO_Initialize();
	//CPU_INTC_Initialize();
	//HAL_Time_Initialize();

    // Added by the samrkash team for bottom half processing
    //Tasklet_Initialize();

	GLOBAL_LOCK(irq);



	GPIO_InitTypeDef GPIO_InitStructure;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 1, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, FALSE);
	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//RCC_MCOConfig(RCC_MCO_SYSCLK);

	Tasklet_Initialize();

	//__SVC();
	//Tasklet_Run();

	testMathInstance.prng_init(30);

	for(UINT32 i = 0; i < 1000; i++)
	{
		compareArray[i] = 	testMathInstance.pareto_prng();
	}

	//g_STM32F10x_AdvancedTimer.Initialize(0, NULL, NULL);

	//Timer_Driver :: Initialize (2 , TRUE, 0, 0, ISR_TIMER, NULL);

	//Timer_Driver :: SetCompare(2 , 1000);
	//while(TRUE);


	m_lastRead = 0;

	m_toggleFlag= FALSE;

};

extern "C"
{
	void Timer_Test_Handler(void *param)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		//UINT32 *counterVal = (UINT32 *) param;

		//if(randomValueCounter < 1000)
		g_STM32F10x_AdvancedTimer.SetCompare(*((UINT32 *)param) , 240000);
		//g_STM32F10x_AdvancedTimer.SetCompare(0 , 240000);
		//else
		//	testComplete = TRUE;
	}
}

// Test the unsigned int theory
BOOL TimerTest::Level_0B()
{

	g_STM32F10x_AdvancedTimer.Initialize(0, Timer_Test_Handler, NULL);

	g_STM32F10x_AdvancedTimer.SetCompare(0 , 240000);

	while(!testComplete)
	{
	   	for(UINT16 i =0 ; i < 1000; i++);
	}

	for(UINT16 i = 0; i < 1000; i++)
	{
		hal_printf("RandomValue,%d\n", compareArray[i]);
	}

	return TRUE;

#if 0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	TIM_TimeBaseStructure.TIM_Period = 10;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/* Slave Mode selection: TIM3 */
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);

	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	 /* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 127;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;


    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    /* Master Mode selection */
  	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

  	/* Select the Master Slave Mode */
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);


	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM1_CC, ISR_TIM1, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TIM1_CC))
		return FALSE;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM2, ISR_TIM2, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TIM2))
		return FALSE;

	TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	//TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	TIM_SetCompare2(TIM1, 20000);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2,ENABLE);

    while(TRUE)
    {
    	for(UINT16 i =0 ; i < 1000; i++);
    }

	return TRUE;
#endif

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

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

	// Initialize the rcc
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 10;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    /* Slave Mode selection: TIM3 */
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);


	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 0xffff;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);


	 /* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 127;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	/* Automatic Output enable, Break, dead time and lock configuration*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	TIM_BDTRInitStructure.TIM_DeadTime = 5;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

	  /* Master Mode selection */
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

	  /* Select the Master Slave Mode */
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);



	TIM_UpdateRequestConfig(TIM1, TIM_UpdateSource_Global);

	TIM_SetAutoreload(TIM1, 0xffff);

	TIM_ARRPreloadConfig(TIM1, ENABLE);


	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM1_CC, ISR_TIM1, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TIM1_CC))
		return FALSE;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TIM2, ISR_TIM2, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TIM2))
		return FALSE;


	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);

	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);

	TIM_Cmd(TIM2,ENABLE);

	/* Main Output Enable */

	TIM_CtrlPWMOutputs(TIM1, ENABLE);


	while(TRUE);

	return TRUE;

}

#if 0
extern "C"
{

void ISR_TIM2(void* Param)
{
	 TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	 CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
     CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
}

void ISR_TIM1( void* Param )
{

	 if(TIM_GetFlagStatus(TIM1, TIM_IT_CC1))
	 {
		 TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		 CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		 CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

	 }
	 else if(TIM_GetFlagStatus(TIM1, TIM_IT_CC2))
	 {
		 TIM_ITConfig(TIM1, TIM_IT_CC2, DISABLE);
		 //TIM_SetCompare2(TIM1, TIM_GetCounter(TIM1) + 20000);
		 TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);

		 CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
		 CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

	 }
	//hal_printf("The time value is %d \n", TIM_GetCounter(TIM1));
 	//hal_printf("The compare register value is %d\n", TIM_GetCapture1(TIM1));


}
}
#endif

BOOL TimerTest::Execute(int level )
{
	if(level == 0)
		return Level_0A();
	else
		return Level_0B();

}; //Execute


//--//


