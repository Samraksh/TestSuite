
#include <tinyhal.h>
#include <tim/netmf_timers.h>
#include "testMath.h"

#ifndef _Timer_Test_
#define _Timer_Test_ 1

class TimerTest
{
    UINT32   m_displayInterval;
    UINT32   m_timerDuration;
    CHAR     thorp[4];

    testMath testMathInstance;

public:

    UINT32   compareArray[1000];



//--//
    		TimerTest ( UINT32 DisplayIntervalSeconds , UINT32 TimerDurationSeconds );

    BOOL    Execute(  );

};

#endif


