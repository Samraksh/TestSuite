


#ifndef _Timer_Test_
#define _Timer_Test_ 1

#define Level_0C_Test 3

#include <tinyhal.h>
#include <rcc/stm32f10x_rcc.h>
#include <tim/netmf_timers.h>
#include <tim/stm32f10x_tim.h>
#include <intc/stm32.h>
#include <gpio/stm32f10x_gpio.h>
#include <advancedtim/netmf_advancedtimer.h>
#include "testMath.h"

class TimerTest
{
	UINT64 m_lastRead;
	BOOL   m_toggleFlag;

    testMath testMathInstance;

public:




//--//
 	TimerTest ( UINT32 DisplayIntervalSeconds , UINT32 TimerDurationSeconds );

    BOOL    Execute(int level );

    BOOL    Level_0A();

    BOOL    Level_0B();

    BOOL    Level_0C();

    //void    ISR(void* param);

};

#endif


