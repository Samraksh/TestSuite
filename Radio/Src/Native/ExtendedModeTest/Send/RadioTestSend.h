/*
 * RadioTest.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>
#include <pal/COM/Wireless/MAC/CSMAMAC/csmaMAC.h>
#include <Targets/Native/STM32F10x/DeviceCode/drivers/radio/RF231/RF231.h>


#define PAYLOAD_SIZE	5

typedef struct  {
	UINT16 MSGID;
	UINT8 data[PAYLOAD_SIZE];
}Payload_t;

class RadioTestSend	{
public:
	Payload_t msg;
	Message_15_4_t msg_carrier;
	UINT8 radioName;
	UINT32 msgID;
	UINT8 currentTry;
	UINT8 retryLimit;

	RadioEventHandler_t Radio_Event_Handler;
	/*bool initialPacketReceived;
	UINT8 MyAppID;
	MacConfig Config;
	UINT8 MacId;
	MacEventHandler myEventHandler;*/

	BOOL Initialize();
	BOOL StartTest();
	void SendPacket();
	void CreatePacket();
	void* Receive(void* msg, UINT16 size);
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status);
};

//extern RadioTest g_RadioTest;

void RadioTest_Initialize();

#endif /* RadioTest_H_ */



