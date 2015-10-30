/*
 * OMACTest.h
 */

#ifndef OMACTEST_H_
#define OMACTEST_H_

#include <tinyhal.h>
#include <Samraksh/Mac_decl.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>


typedef struct  {
	UINT32 MSGID;
	char* msgContent;
}Payload_t_ping;


class OMACTest{
public:
	UINT8 MyAppID;
	Payload_t_ping pingPayload;
	MacEventHandler myEventHandler;
	MacConfig Config;
	UINT8 MacId;
	static UINT32 sendPingCount;
	static UINT32 recvCount;
	static UINT32 missedPingID;
	UINT16 RcvCount;

	BOOL Initialize();
	BOOL StartTest();
	void Receive(void* msg, UINT16 size);
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status);
	void GetStatistics();
};

//extern OMACTest g_OMACTest;

void OMACTest_Initialize();

UINT32 OMACTest::sendPingCount = 0;
UINT32 OMACTest::recvCount = 0;
UINT32 OMACTest::missedPingID = 1;

#endif /* OMACTEST_H_ */



