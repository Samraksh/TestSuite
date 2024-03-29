////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"


#define LEVEL_0A 0
#define LEVEL_0B 1
#define LEVEL_0C 2
#define LEVEL_1  3

class EventsTest
{

	int numberOfEvents;

	testMath testMathInstance;

public:

//--//
    EventsTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     Level_0C();

	BOOL 	 Level_1();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





