


#ifndef _Timer_Test_
#define _Timer_Test_ 1

#define Level_0C_Test 3

#include <tinyhal.h>
#include <tim/netmf_timers.h>

class TimerTest
{
	UINT64 m_lastRead;
	BOOL   m_toggleFlag;


public:

//--//
 	TimerTest ( UINT32 DisplayIntervalSeconds , UINT32 TimerDurationSeconds );

    BOOL    Execute(int level );

    BOOL    Level_0A();

    BOOL    Level_0B();

    BOOL    Level_0C();

    void    ISR(void* param);

};

#endif


