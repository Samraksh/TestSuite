////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <intc/STM32.h>
#include <tim/netmf_timers.h>
#include <time/netmf_time.h>

#define LEVEL_0A 0
#define LEVEL_0B 1

class INTC
{

public:

//--//
    //INTC ( int seedValue, int numberOfEvents );
	BOOL    Initialize(int seedValue, int numberOfEvents);

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

};


