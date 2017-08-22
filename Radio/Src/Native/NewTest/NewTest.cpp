
#include "NewTest.h"


const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

TestObject_t g_NewTestObject_t;



/*
 * Start TEST_0A_TIMER1 which keeps sending a packet until a response is received
 */
BOOL TestObject_t::StartTest()
{

	return TRUE;
}

BOOL TestObject_t::Initialize()
{



	return TRUE;
}

void Test_InitializeAndRun()
{
	g_NewTestObject_t.Initialize();
	g_NewTestObject_t.StartTest();
}


