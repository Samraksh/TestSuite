/*
 * RadioTestReceive.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>
#include <../DeviceCode/Include/Samraksh/MAC/CSMAMAC/csmaMAC.h>
#include <Targets/Native/STM32F10x/DeviceCode/drivers/radio/RF231/RF231.h>
//#include <MicroFrameworkPK_v4_3/DeviceCode/Include/Samraksh/Radio_decl.h>


#define MAX_NEIGHBORS 	12
#define PAYLOAD_SIZE	5

typedef struct  {
	UINT16 MSGID;
	UINT8 data[PAYLOAD_SIZE];
}Payload_t;

typedef struct{
	UINT16 nodeID;
	UINT32 lastMSGID;
	UINT32 totalRecvCount;
}NodeStats_t;

typedef struct{
	NodeStats_t nodeStats;
}NewNode_t;


class RadioTestReceive	{
public:
	Payload_t msg;
	NewNode_t newNode[MAX_NEIGHBORS];
	UINT8 radioName;
	bool initialPacketReceived;
	Message_15_4_t msg_carrier;
	UINT8 MyAppID;
	MACConfig Config;
	UINT8 MACId;
	UINT16 myAddress;
	UINT16 SendCount, receiveCount;
	static UINT8 nodeCount;
	static UINT64 startTicks, endTicks;
	MACEventHandler myEventHandler;
	RadioEventHandler_t Radio_Event_Handler;

	BOOL DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);
	BOOL Initialize();
	BOOL StartTest();
	void SendPacket();
	void VerifyCCA();
	Message_15_4_t CreatePacket();
	int LookupNodeID(UINT16 nodeID);
	void* Receive(void* msg, UINT16 size);
	void ShowStats();
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus);
};

//extern RadioTestReceive g_RadioTestReceive;

void RadioTest_Initialize();

#endif /* RadioTest_H_ */



