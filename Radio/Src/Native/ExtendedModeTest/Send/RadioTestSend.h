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

typedef struct UDP_Header
{
	UINT16 srcPort;
	UINT16 destPort;
	UINT16 length;
	UINT16 checkSum;
	UINT8 payload[50];
}UDP_Header_t;

typedef struct ZEP_Header
{
	UINT8 hf_zep_version = -1;
	UINT8 hf_zep_channel_id = -1;
	UINT16 hf_zep_device_id = -1;
	BOOL hf_zep_lqi_mode = -1;
	UINT8 hf_zep_lqi = -1;
	UINT8 hf_zep_reserved_field[7] = {-1};
	UINT8 hf_zep_ieee_length = -1;
	//UINT8 hf_zep_seqno = -1;
	//UINT32 hf_zep_timestamp = -1;
	//UINT8 hf_zep_type = -1;
}ZEP_Header_t;

class RadioTestSend	{
public:
	Payload_t msg;
	Message_15_4_t msg_carrier;
	ZEP_Header_t zepHeader;
	UDP_Header_t udp_header;
	UINT8 radioName;
	UINT32 msgID;
	UINT8 currentTry;
	UINT8 retryLimit;
	UINT16 myAddress;
	UINT8 finalSeqNumber;

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



