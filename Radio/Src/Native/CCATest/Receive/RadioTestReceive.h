/*
 * RadioTestReceive.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>
#include <pal/COM/Wireless/MAC/CSMAMAC/csmaMAC.h>
#include <Targets/Native/STM32F10x/DeviceCode/drivers/radio/RF231/RF231.h>
//#include <MicroFrameworkPK_v4_3/DeviceCode/Include/Samraksh/Radio_decl.h>

#define PAYLOAD_SIZE	5

typedef struct  {
	UINT16 MSGID;
	UINT8 data[PAYLOAD_SIZE];
}Payload_t;

class RadioTestReceive	{
public:
	Payload_t msg;
	UINT8 radioName;
	bool initialPacketReceived;
	Message_15_4_t msg_carrier;
	UINT8 MyAppID;
	MacConfig Config;
	UINT8 MacId;
	MacEventHandler myEventHandler;

	RadioEventHandler_t Radio_Event_Handler;
	BOOL Initialize();
	BOOL StartTest();
	void SendPacket();
	void VerifyCCA();
	Message_15_4_t CreatePacket();
	void* Receive(void* msg, UINT16 size);
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status);
};

//extern RadioTestReceive g_RadioTestReceive;

void RadioTest_Initialize();

#endif /* RadioTest_H_ */



