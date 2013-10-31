////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"
#include <stm32f10x.h>
#include <intc\STM32.h>
#include <gpio\stm32f10x_gpio.h>
#include <dma\stm32f10x_dma.h>
#include <tim\stm32f10x_tim.h>
#include <adc\stm32f10x_adc.h>
#include <rcc\stm32f10x_rcc.h>

#define LEVEL_0A 0
#define LEVEL_0B 1
#define LEVEL_0C 2
#define LEVEL_0D 3
#define LEVEL_1  4

class ADCTest
{

	int numberOfEvents;

	testMath testMathInstance;

public:

//--//
    ADCTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     Level_0C();

	BOOL     Level_0D();

	BOOL 	 Level_1();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





