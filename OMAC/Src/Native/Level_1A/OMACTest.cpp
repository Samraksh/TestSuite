/*
 * TimesyncTest.cpp
 */

/*
 * OMACTest.cpp
 *
 *  Created on: Oct 8, 2012
 *      Author: Mukundan Sridharan
 *      A simple Common test for all MACs
 */

//#include <Samraksh/HALTimer.h>
#include "OMACTest.h"

//#define DEBUG_OMACTest 1

const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

OMACTest g_OMACTest;
extern NeighborTable g_NeighborTable;
extern OMACType g_OMAC;
extern OMACScheduler g_omac_scheduler;
extern UINT16 MF_NODE_ID;
extern Buffer_15_4_t g_send_buffer;
extern DataTransmissionHandler g_DataTransmissionHandler;
extern DataReceptionHandler g_DataReceptionHandler;

UINT32 txCounter = 0;


void Timer_32_Handler(void * arg){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
#endif
	txCounter++;
	g_OMACTest.Send();
	hal_printf("---------%u--------\n\n", txCounter);

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
#endif
}

// Typedef defining the signature of the receive function
//void ReceiveHandler (void *msg, UINT16 Size, UINT16 Src, BOOL Unicast, UINT8 RSSI, UINT8 LinkQuality){
void OMACTest_ReceiveHandler (void *msg, UINT16 size){
	return g_OMACTest.Receive(msg, size);
}

void OMACTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
	g_OMACTest.SendAck(msg,size,status);
}

BOOL OMACTest::Initialize(){
	//MF_NODE_ID=10;	//Set address
	MyAppID = 3; //pick a number less than MAX_APPS currently 4.
	Config.Network = 138;
	Config.NeighborLivenessDelay = 900000;
	myEventHandler.SetReceiveHandler(OMACTest_ReceiveHandler);
	myEventHandler.SetSendAckHandler(OMACTest_SendAckHandler);
	MacId = OMAC;

	VirtTimer_Initialize();
#ifdef DEBUG_OMACTest
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, FALSE);
#endif
	Mac_Initialize(&myEventHandler, MacId, MyAppID, Config.RadioID, (void*) &Config);

	VirtTimer_SetTimer(32, 0, 5*ONEMSEC_IN_USEC*ONESEC_IN_MSEC, FALSE, FALSE, Timer_32_Handler); //period (3rd argument) is in micro seconds

	//<start> Initialize payload
	for(int i = 1; i <= payloadSize; i++){
		pingPayload.data[i-1] = i;
	}
	pingPayload.MSGID = sendPingCount;
	pingPayload.msgContent = (char*)"PING";

	//pongPayload = (Payload_t_pong*)private_malloc(sizeof(Payload_t_pong));
	//pingPayload.pongPayload = pongPayload;
	pingPayload.pongPayload.MSGID = 0;
	pingPayload.pongPayload.msgContent = (char*)"NULL";
	//<end> Initialize payload

	return TRUE;
}

BOOL OMACTest::StartTest(){
	RcvCount = 0;
	//while(1){
		VirtTimer_Start(32);
	//}

	return TRUE;
}

static bool MyStrCmp(char* str1, char* str2)
{
	bool retVal = false;
	int i = 0;
	while(str1[i] != '\0' && str2[i] != '\0')
	{
		if(str1[i] != str2[i]){
			retVal = false;
			break;
		}
		else {
			retVal = true;
		}
		i++;
	}
	return retVal;
}

void OMACTest::Receive(void* tmpMsg, UINT16 size){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN)30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)30, FALSE);
#endif

	hal_printf("start OMACTest::Receive. I am node: %d\n", g_OMAC.GetAddress());
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	hal_printf("OMACTest src is %u\n", rcvdMsg->GetHeader()->src);
	hal_printf("OMACTest dest is %u\n", rcvdMsg->GetHeader()->dest);

	Payload_t_ping* payload = (Payload_t_ping*)rcvdMsg->GetPayload();
	UINT16 msgId = payload->MSGID;
	hal_printf("OMACTest msgID: %d\n", msgId);
	hal_printf("OMACTest payload is \n");
	for(int i = 1; i <= payloadSize; i++){
		hal_printf(" %d\n", payload->data[i-1]);
	}
	hal_printf("ping msgContent: %s\n", payload->msgContent);

	hal_printf("pong msgID: %d\n", payload->pongPayload.MSGID);
	hal_printf("pong msgContent: %s\n", payload->pongPayload.msgContent);

	//When a node X sends a "PING", it is expecting to hear back a "PONG" with same msgID from node Y.
	//Node X, on hearing a "PONG", checks if PONG msgID is same as PING msgID.
	//	If yes, increments global static msgID and sends a new "PING".
	//	If no, keeps sending old PING.
	//
	//Continue to send a "PONG", until "PING" msgID from other node is +1 of current PONG.
	//If yes, then send a new PONG
	if(MyStrCmp(payload->msgContent, (char*)"PING")){
		if(payload->pongPayload.MSGID == 0){
			pingPayload.pongPayload.MSGID = sendPongCount;
			hal_printf("1. Sending a pong. msgId is %d\n", pingPayload.pongPayload.MSGID);
			pingPayload.pongPayload.msgContent = (char*)"PONG";
		}
		//When a node gets a ping, check if its msgId is +1 of pong msgId.
		//If yes, then it means that a pong reached the other node and that the other node
		//is responding with a new ping.
		if(payload->MSGID == sendPongCount+1){
			ASSERT(sendPongCount+1 == sendPingCount);
			sendPongCount = sendPingCount;
			pingPayload.pongPayload.MSGID = sendPongCount;
			hal_printf("2. Sending a pong. msgId is %d\n", pingPayload.pongPayload.MSGID);
			pingPayload.pongPayload.msgContent = (char*)"PONG";
		}
		pingPayload.MSGID = sendPingCount;
		hal_printf("Sending a ping. msgId is %d\n", pingPayload.MSGID);
		pingPayload.msgContent = (char*)"PING";
	}

	//When a node gets a pong, check if its msgId matches the current ping.
	//If yes, then it means that the ping was reached.
	//Increase the ping msgID.
	if(MyStrCmp(payload->pongPayload.msgContent, (char*)"PONG")){
		if(payload->pongPayload.MSGID == sendPingCount){
			sendPingCount++;
			pingPayload.MSGID = sendPingCount;
			hal_printf("Sending a ping. msgId is %d\n", pingPayload.MSGID);
			pingPayload.msgContent = (char*)"PING";
		}
	}

	hal_printf("\n");

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
	hal_printf("end OMACTest::Receive\n");
}

void OMACTest::SendAck(void *msg, UINT16 size, NetOpStatus status){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 31, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 31, FALSE);
#endif
	if(status == NO_Success){

	}else {

	}
}


BOOL OMACTest::Send(){
	UINT16 Neighbor2beFollowed = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed;
	if (g_NeighborTable.GetNeighborPtr(Neighbor2beFollowed) == NULL) {
		return FALSE;
	}

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
#endif

	hal_printf("ping msgId before sending is %d\n", pingPayload.MSGID);
	hal_printf("pong msgId before sending is %d\n", pingPayload.pongPayload.MSGID);
	bool ispcktScheduled = Mac_Send(Neighbor2beFollowed, MFM_DATA, (void*) &pingPayload, sizeof(Payload_t_ping));
}

void OMACTest_Initialize(){
	g_OMACTest.Initialize();
	g_OMACTest.StartTest();
}


