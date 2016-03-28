/*
 * OMACTest.h
 */

#ifndef OMACTEST_H_
#define OMACTEST_H_

#include <tinyhal.h>
//#include <../DeviceCode/Targets/Native/STM32F10x/DeviceCode/drivers/radio/RF231/RF231.h>
#include <Samraksh/MAC_decl.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>

typedef struct  {
	UINT16 MSGID;
	UINT8 data[5];
}Payload_t;

enum RadioID : UINT8
{
	RF231RADIO,
	RF231RADIOLR,
};

class OMACTest{
public:
	UINT8 MyAppID;
	Payload_t msg;
	MACEventHandler myEventHandler;
	MACConfig Config;
	UINT8 MACId;
	UINT16 SendCount, receiveCount;

	BOOL Initialize();
	BOOL StartTest();
	void Receive(void* msg, UINT16 size);
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus);
};

//extern OMACTest g_OMACTest;

void OMACTest_Initialize();

#endif /* OMACTEST_H_ */



