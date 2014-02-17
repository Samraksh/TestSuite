////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include "testMath.h"
#include <tinyhal.h>
#include <CPU_SPI_decl.h>
#include <..\Targets\Native\STM32F10x\DeviceCode\drivers\spi\netmf_spi.h>
#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include "spi/netmf_spi.h"
//#include <time.h>
//#include <cerrno>

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
*/

/*
#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
//#define sleep(x) Sleep(1000 * x)
#endif
*/

#define LEVEL_0A	0
#define LEVEL_0B	1


class SPI
{

	//int numberOfEvents;
	//testMath testMathInstance;

public:

//--//
    //SPI ( int seedValue, int numberOfEvents );
    SPI ();

    BOOL	Execute( int testLevel );

	BOOL	Level_0A();

	BOOL	Level_0B();

	BOOL	DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};


