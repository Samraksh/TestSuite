////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"


#define LEVEL_0A 0
#define LEVEL_0B 1
#define LEVEL_0C 2
#define LEVEL_1  3

class MACLayerTest
{

	int numberOfEvents;


	testMath testMathInstance;

public:

	static BOOL SendAckPending;

	static UINT8 MacID;

//--//
    MACLayerTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     Level_0C();

	BOOL 	 Level_1();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, int resultParameter2, int resultParameter3, int accuracy);

};

BOOL MACLayerTest::SendAckPending = FALSE;
UINT8 MACLayerTest::MacID = 0;




