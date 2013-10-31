


#ifndef _GlobalLock_Test_
#define _GlobalLock_Test_ 1

#include <tinyhal.h>
#include <tim/netmf_timers.h>

class GlobalLockTest
{
	UINT64 m_lastRead;
	BOOL   m_toggleFlag;

	UINT32 numberOfEvents;


public:

//--//
 	GlobalLockTest ( UINT32 DisplayIntervalSeconds , UINT32 GlobalLockDurationSeconds );

    BOOL    Execute(int level );

    BOOL    Level_0A();

    BOOL    Level_0B();

    void    ISR(void* param);
	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};

#endif


