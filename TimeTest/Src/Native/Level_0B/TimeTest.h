////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

class TimeTest
{

public:

//--//
	static UINT64 prevTicks;
	TimeTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};


