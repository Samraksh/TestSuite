////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "csmaMACTest.h"
#include <..\pal\COM\Radio\MAC\CSMAMAC\csmaMAC.h>
//#include <..\pal\COM\Radio\MAC\OMAC\OMAC.h>

//extern OMAC g_OMAC;
//---//
extern csmaMAC gcsmaMacObject;

csmaMACTest::csmaMACTest( int seedValue, int numberOfEvents )
{
	gcsmaMacObject.Initialize();
	
};

BOOL csmaMACTest::Level_0A()
{
	UINT8 sendmsg[] = {0,1,0,2,0,3,0,4,0,5,0,6};
	while(true)
	{
		gcsmaMacObject.Send(sendmsg, 12);
		for(int i = 0; i < 1500; i++);
	}
	return TRUE;

}

BOOL csmaMACTest::Level_0B()
{

}




BOOL csmaMACTest::Execute( int testLevel )
{
	if(testLevel == LEVEL_0A)
		Level_0A();
} //Execute

