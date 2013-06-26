////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"
#include <fsmc/P30BF65NOR/P30BF65NOR.h>

#define LEVEL_0A 0
#define LEVEL_0B 1
#define LEVEL_0C 2
#define LEVEL_0D 3
#define LEVEL_1  4

class NORTest
{

	int numberOfEvents;

	testMath testMathInstance;

public:

//--//
    NORTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     Level_0C();

	BOOL     Level_0D();

	BOOL 	 Level_1();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};





