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

const char payloadSize = 5;

typedef struct  {
	UINT16 MSGID;
	UINT8 data[payloadSize];
	char* msgContent;
}Payload_t;

class OMACTest{
public:
	UINT8 MyAppID;
	Payload_t msg;
	MacEventHandler myEventHandler;
	MacConfig Config;
	UINT8 MacId;
	UINT16 SendCount;
	UINT16 RcvCount;

	BOOL Initialize();
	BOOL StartTest();
	void Receive(void* msg, UINT16 size);
	BOOL Send_fan_in();
	BOOL Send_fan_out();
	void SendAck(void *msg, UINT16 size, NetOpStatus status);
};

//extern OMACTest g_OMACTest;

void OMACTest_Initialize();

#endif /* OMACTEST_H_ */



