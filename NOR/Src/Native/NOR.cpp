////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "NOR.h"

//---//

extern P30BF65NOR_Driver gNORDriver;

NORTest::NORTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	gNORDriver.Initialize();

};

BOOL NORTest::Level_0A()
{

	while(true)
	{
		gNORDriver.ReadID();

		for(UINT16 i = 0; i < 10000; i++);
	}


	return TRUE;

}

BOOL NORTest::Level_0B()
{
}


BOOL NORTest::Execute( int testLevel )
{
	BOOL result;

	if(testLevel == LEVEL_0)
	{
		// result = SleepTest_Level0();
		// result &= SleepTest_Level1A();
		// result &= SleepTest_Level1B();
		 result = Level_0A();
	}

	return result;



} //Execute

