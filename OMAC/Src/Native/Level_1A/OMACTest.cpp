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

const UINT32 endOfTestCounter = 100;
const UINT32 delayBetweenPackets = 5;	//in secs


void Timer_32_Handler(void * arg){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
#endif
	g_OMACTest.Send();
	hal_printf("---------%u--------\n\n", g_OMACTest.sendPingCount);

	if(g_OMACTest.sendPingCount == endOfTestCounter){
		VirtTimer_Stop(32);
		g_OMACTest.GetStatistics();
	}

	g_OMACTest.sendPingCount++;

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

void OMACTest::GetStatistics()
{
	hal_printf("OMACTest Level_1A. I am node: %d\n", g_OMAC.GetAddress());
	hal_printf("Total packets transmitted: %u\n", sendPingCount);
	hal_printf("Total packets received: %u\n", recvCount);
	hal_printf("percentage received: %d %\n", (recvCount/sendPingCount)*100);
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

	VirtTimer_SetTimer(32, 0, delayBetweenPackets*ONEMSEC_IN_USEC*ONESEC_IN_MSEC, FALSE, FALSE, Timer_32_Handler); //period (3rd argument) is in micro seconds

	//<start> Initialize payload
	pingPayload.MSGID = sendPingCount;
	pingPayload.msgContent = (char*)"PING";
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


void OMACTest::Receive(void* tmpMsg, UINT16 size){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN)30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)30, FALSE);
#endif

	hal_printf("OMACTest::Receive start. I am node: %d\n", g_OMAC.GetAddress());
	recvCount++;
	hal_printf("OMACTest::Receive recvCount is %d\n", recvCount);
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	hal_printf("OMACTest src is %u\n", rcvdMsg->GetHeader()->src);
	hal_printf("OMACTest dest is %u\n", rcvdMsg->GetHeader()->dest);

	Payload_t_ping* payload = (Payload_t_ping*)rcvdMsg->GetPayload();
	hal_printf("PING msgID: %d\n", payload->MSGID);
	hal_printf("PING msgContent: %s\n", payload->msgContent);

	pingPayload.MSGID = sendPingCount;
	pingPayload.msgContent = (char*)"PING";

	hal_printf("\n");

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
	hal_printf("OMACTest::Receive end\n");
}

void OMACTest::SendAck(void *msg, UINT16 size, NetOpStatus status){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 31, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 31, FALSE);
#endif
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	Payload_t* payload = (Payload_t*)rcvdMsg->GetPayload();
	if(status == NO_Success){
		hal_printf("OMACTest::SendAck sent msgID: %d from source %d to dest %d\n", payload->MSGID, rcvdMsg->GetHeader()->src, rcvdMsg->GetHeader()->dest);
	}else {
		hal_printf("OMACTest::SendAck Error while sending msgID: %d from source %d to dest %d\n", payload->MSGID, rcvdMsg->GetHeader()->src, rcvdMsg->GetHeader()->dest);
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
	bool ispcktScheduled = Mac_Send(Neighbor2beFollowed, MFM_DATA, (void*) &pingPayload, sizeof(Payload_t_ping));
}

void OMACTest_Initialize(){
	g_OMACTest.Initialize();
	g_OMACTest.StartTest();
}


