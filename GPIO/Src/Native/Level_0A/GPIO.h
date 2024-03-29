
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#define SUCCESS 1
#define FAIL 0

#define LEVEL_0_A 0
#define LEVEL_0_B 1
#define LEVEL_1_A 2
#define LEVEL_1_B 3
#define LEVEL_2	4

class GPIO
{
	public:
		GPIO (  );
		BOOL     Execute( int testLevel );
    		BOOL 	 Level_0A();
		BOOL 	 Level_0B();
		BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);
};

