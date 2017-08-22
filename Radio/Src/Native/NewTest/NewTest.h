/*
 * RadioTest.h
 */

#ifndef NewTest_H_
#define NewTest_H_

#include <tinyhal.h>


class TestObject_t	{

public:

	BOOL Initialize();
	BOOL StartTest();
	void ScheduleSendPacket();

};

//extern RadioTest g_RadioTest;

void Test_InitializeAndRun();

#endif /* RadioTest_H_ */



