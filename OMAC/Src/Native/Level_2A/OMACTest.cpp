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

#define TIMER_Counter_UpperBound	100
#define FAN_IN_TIMER	32
#define FAN_OUT_TIMER	33

static UINT32 FAN_IN_txCounter = 0;
static UINT32 FAN_OUT_txCounter = 0;

UINT16 FAN_IN_TIMER_Counter = 0;
UINT16 FAN_OUT_TIMER_Counter = 0;


//1st 2 nodes are either Tx or Rx nodes depending on whether mode is fan_in or fan_out
typedef struct
{
	UINT32 Neighbor2beFollowed1;
	UINT32 Neighbor2beFollowed2;
	UINT32 Neighbor2beFollowed;
}FanInOutNodes;


void GetFanInOutNodes(char* mode, FanInOutNodes* fanInOutNodes)
{
	if(hal_stricmp(mode, (char*)"FAN_IN") == 0){
		/*if(g_OMAC.GetAddress() == 3505) {
			fanInOutNodes->Neighbor2beFollowed = 6846;
		}
		else if(g_OMAC.GetAddress() == 31436) {
			fanInOutNodes->Neighbor2beFollowed = 6846;
		}
		else if(g_OMAC.GetAddress() == 6846) {
			fanInOutNodes->Neighbor2beFollowed = 6846;
			fanInOutNodes->Neighbor2beFollowed1 = 3505;
			fanInOutNodes->Neighbor2beFollowed2 = 31436;
		}*/
		fanInOutNodes->Neighbor2beFollowed = 6846;
		fanInOutNodes->Neighbor2beFollowed1 = 3505;
		fanInOutNodes->Neighbor2beFollowed2 = 31436;
	}
	else if(hal_stricmp(mode, (char*)"FAN_OUT") == 0){
		/*if(g_OMAC.GetAddress() == 3505) {
			fanInOutNodes->Neighbor2beFollowed = 6846;
		}
		else if(g_OMAC.GetAddress() == 31436) {
			fanInOutNodes->Neighbor2beFollowed = 6846;
		}
		else if(g_OMAC.GetAddress() == 6846) {
			fanInOutNodes->Neighbor2beFollowed = 6846;
			fanInOutNodes->Neighbor2beFollowed1 = 3505;
			fanInOutNodes->Neighbor2beFollowed2 = 31436;
		}*/
		fanInOutNodes->Neighbor2beFollowed = 6846;
		fanInOutNodes->Neighbor2beFollowed1 = 3505;
		fanInOutNodes->Neighbor2beFollowed2 = 31436;
	}
}


void FAN_IN_TIMER_Handler(void* arg){
	FAN_IN_txCounter++;
	g_OMACTest.Send_fan_in();
	FAN_IN_TIMER_Counter++;
	hal_printf("---------%u--------\n\n", FAN_IN_txCounter);
}

void FAN_OUT_TIMER_Handler(void* arg){
	FAN_OUT_txCounter++;
	g_OMACTest.Send_fan_out();
	FAN_OUT_TIMER_Counter++;
	hal_printf("---------%u--------\n\n", FAN_OUT_txCounter);
}

// Typedef defining the signature of the receive function
//void ReceiveHandler (void *msg, UINT16 Size, UINT16 Src, BOOL Unicast, UINT8 RSSI, UINT8 LinkQuality){
void OMACTest_ReceiveHandler (void* msg, UINT16 size){
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

	VirtTimer_SetTimer(FAN_IN_TIMER, 0, 2*ONEMSEC_IN_USEC*ONESEC_IN_MSEC, FALSE, FALSE, FAN_IN_TIMER_Handler); //period (3rd argument) is in micro seconds
	VirtTimer_SetTimer(FAN_OUT_TIMER, 0, 2*ONEMSEC_IN_USEC*ONESEC_IN_MSEC, FALSE, FALSE, FAN_OUT_TIMER_Handler); //period (3rd argument) is in micro seconds
	return TRUE;
}


BOOL OMACTest::StartTest(){
	//msg.MSGID = 0;
	SendCount = 1;
	RcvCount = 0;
	hal_printf(">>>>>>>>>>>>STARTING FAN_IN<<<<<<<<<<<<\n");
	VirtTimer_Start(FAN_IN_TIMER);
	while(1)
	{
		if(FAN_IN_TIMER_Counter == TIMER_Counter_UpperBound && FAN_OUT_TIMER_Counter == 0){
			VirtTimer_Stop(FAN_IN_TIMER);
			FAN_IN_TIMER_Counter = 0;
			hal_printf("<<<<<<<<<<<<<<<<STARTING FAN_OUT>>>>>>>>>>>>>>>>\n");
			VirtTimer_Start(FAN_OUT_TIMER);
		}
		else if(FAN_IN_TIMER_Counter == 0 && FAN_OUT_TIMER_Counter == TIMER_Counter_UpperBound){
			VirtTimer_Stop(FAN_OUT_TIMER);
			FAN_OUT_TIMER_Counter = 0;
			hal_printf(">>>>>>>>>>>>STARTING FAN_IN<<<<<<<<<<<<\n");
			VirtTimer_Start(FAN_IN_TIMER);
		}
	}

	return TRUE;
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

	Payload_t* payload = (Payload_t*)rcvdMsg->GetPayload();
	UINT16 msgId = payload->MSGID;
	hal_printf("OMACTest msgID: %d\n", msgId);
	hal_printf("OMACTest payload is \n");
	for(int i = 1; i <= payloadSize; i++){
		hal_printf(" %d\n", payload->data[i-1]);
	}
	hal_printf("msgContent: %s\n", payload->msgContent);

	if(hal_stricmp(payload->msgContent, (char*)"PING") == 0){
		hal_printf("Inside ping\n");
		msgId += 1;
		msg.MSGID = msgId;
		hal_printf("msgId inside ping is %d\n", msg.MSGID);
		msg.msgContent = (char*)"PONG";
	}
	else if(hal_stricmp(payload->msgContent, (char*)"PONG") == 0){
		hal_printf("Inside pong\n");
		msgId += 2;
		msg.MSGID = msgId;
		hal_printf("msgId inside pong is %d\n", msg.MSGID);
		msg.msgContent = (char*)"PING";
	}
	else {
		hal_printf("something wrong\n");
	}

	hal_printf("\n");

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
	hal_printf("end OMACTest::Receive\n");
}


void OMACTest::SendAck(void* tmpMsg, UINT16 size, NetOpStatus status){
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

//Multiple nodes sending to one common node
BOOL OMACTest::Send_fan_in(){
	static bool flag = false;
	bool ispcktScheduled = false;
	FanInOutNodes fanInNodes;

	GetFanInOutNodes((char*)"FAN_IN", &fanInNodes);
	//UINT16 Neighbor2beFollowed = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed;
	UINT16 Neighbor2beFollowed = fanInNodes.Neighbor2beFollowed;

	for(int i = 1; i <= payloadSize; i++){
		msg.data[i-1] = i;
	}
	if(!flag){
		msg.MSGID = SendCount;
		msg.msgContent = (char*)"PING";
		flag = true;
		if(g_OMAC.GetAddress() != Neighbor2beFollowed)
			hal_printf("FAN IN; Sending to node %d from node %d\n", Neighbor2beFollowed, g_OMAC.GetAddress());
	}

	if(g_OMAC.GetAddress() == Neighbor2beFollowed){
		hal_printf("I am node %d. Cannot send to myself\n", Neighbor2beFollowed);
		return false;
	}

	if (g_NeighborTable.GetNeighborPtr(Neighbor2beFollowed) == NULL) {
		hal_printf("OMACTest::Send_fan_in Neighbor2beFollowed: %d not found\n", Neighbor2beFollowed);
		//return FALSE;
	}

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
#endif
	hal_printf("Sending from node %d to node %d; msgID is %d\n", g_OMAC.GetAddress(), Neighbor2beFollowed, msg.MSGID);
	ispcktScheduled = Mac_Send(Neighbor2beFollowed, MFM_DATA, (void*) &msg, sizeof(Payload_t));
	if(!ispcktScheduled){
		hal_printf("Send to neighbor node %d failed \n", Neighbor2beFollowed);
	}
}

//One node sending to multiple nodes
BOOL OMACTest::Send_fan_out(){
	static bool flag = false;
	bool ispcktScheduled = false;
	FanInOutNodes fanOutNodes;

	GetFanInOutNodes((char*)"FAN_OUT", &fanOutNodes);
	//UINT16 Neighbor2beFollowed1 = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed1;
	//UINT16 Neighbor2beFollowed2 = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed2;
	UINT16 Neighbor2beFollowed = fanOutNodes.Neighbor2beFollowed;
	UINT16 Neighbor2beFollowed1 = fanOutNodes.Neighbor2beFollowed1;
	UINT16 Neighbor2beFollowed2 = fanOutNodes.Neighbor2beFollowed2;

	for(int i = 1; i <= payloadSize; i++){
		msg.data[i-1] = i;
	}
	if(!flag){
		msg.MSGID = SendCount;
		msg.msgContent = (char*)"PING";
		flag = true;
		if(g_OMAC.GetAddress() != Neighbor2beFollowed)
			hal_printf("FAN OUT; Sending to nodes %d and %d\n", Neighbor2beFollowed1, Neighbor2beFollowed2);
	}

	if(g_OMAC.GetAddress() == Neighbor2beFollowed1 || g_OMAC.GetAddress() == Neighbor2beFollowed2){
		hal_printf("I am node %d. Cannot send to myself\n", g_OMAC.GetAddress());
		return false;
	}

	if (g_NeighborTable.GetNeighborPtr(Neighbor2beFollowed1) == NULL) {
		hal_printf("OMACTest::Send_fan_out Neighbor2beFollowed1: %d not found\n", Neighbor2beFollowed1);
		//return FALSE;
	}
	if (g_NeighborTable.GetNeighborPtr(Neighbor2beFollowed2) == NULL) {
		hal_printf("OMACTest::Send_fan_out Neighbor2beFollowed2: %d not found\n", Neighbor2beFollowed2);
		//return FALSE;
	}

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
#endif
	hal_printf("msgId is %d\n", msg.MSGID);
	hal_printf("Sending from node %d to node %d\n", g_OMAC.GetAddress(), Neighbor2beFollowed1);
	ispcktScheduled = Mac_Send(Neighbor2beFollowed1, MFM_DATA, (void*) &msg, sizeof(Payload_t));
	if(!ispcktScheduled){
		hal_printf("Send to neighbor node %d failed \n", Neighbor2beFollowed1);
	}
	hal_printf("Sending from node %d to node %d\n", g_OMAC.GetAddress(), Neighbor2beFollowed2);
	ispcktScheduled = Mac_Send(Neighbor2beFollowed2, MFM_DATA, (void*) &msg, sizeof(Payload_t));
	if(!ispcktScheduled){
		hal_printf("Send to neighbor node %d failed \n", Neighbor2beFollowed2);
	}
}

void OMACTest_Initialize(){
	g_OMACTest.Initialize();
	g_OMACTest.StartTest();
}


