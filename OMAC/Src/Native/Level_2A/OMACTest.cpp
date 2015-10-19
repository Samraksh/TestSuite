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

Payload_t* g_payload;
bool setMsgReceivedAckFlag = false;



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

void DeepCopy(Payload_t* tmpPayload)
{
	g_payload->MSGID = tmpPayload->MSGID;
	/*memcpy(g_payload->data, tmpPayload->data, payloadSize);
	for(int i = 0; i < payloadSize; i++){
		g_payload->data[i] = tmpPayload->data[i];
	}*/
	for(int i = 1; i <= payloadSize; i++){
		g_payload->data[i-1] = i;
	}
	g_payload->msgContent = tmpPayload->msgContent;
}

static void OMACTest_ReceiveAckHandler (Message_15_4_t* tmpMsg){
	UINT16 Neighbor2beFollowed = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed;
	//Message_15_4_t* msg = (Message_15_4_t*)tmpMsg;
	//memcpy(msg, tmpMsg, sizeof(tmpMsg));
	if(Neighbor2beFollowed == tmpMsg->GetHeader()->src){
		//g_payload = (Payload_t*)msg->GetPayload();
		//memcpy(g_payload, tmpMsg->GetPayload(), sizeof(tmpMsg->GetPayload()));
		DeepCopy((Payload_t*)tmpMsg->GetPayload());
		g_payload->msgContent = (char*)"MSG RECEIVED. THANKS!";
		setMsgReceivedAckFlag = true;
		//Mac_Send(g_OMACTest.MacId, Neighbor2beFollowed, MFM_DATA, (void*) &payload, sizeof(Payload_t));
	}
	else{
		hal_printf("Msg not received from node %d yet\n", Neighbor2beFollowed);
	}
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
	return TRUE;
}



BOOL OMACTest::StartTest(){
	//msg.MSGID = 0;
	SendCount = 1;
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
	OMACTest_ReceiveAckHandler(rcvdMsg);

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
	//char* tmpMsgContent = (char*)private_malloc(payloadSize);
	//char* tmpMsgContent = msg.msgContent;
	//if(MyStrCmp(payload->msgContent, (char*)"PING")){
	if(hal_stricmp(payload->msgContent, (char*)"PING") == 0){
		hal_printf("Inside ping\n");
		msgId += 1;
		msg.MSGID = msgId;
		hal_printf("msgId inside ping is %d\n", msg.MSGID);
		//tmpMsgContent = (char*)"PONG";
		msg.msgContent = (char*)"PONG";
	}
	//else if(MyStrCmp(payload->msgContent, (char*)"PONG")){
	else if(hal_stricmp(payload->msgContent, (char*)"PONG") == 0){
		hal_printf("Inside pong\n");
		msgId += 2;
		msg.MSGID = msgId;
		hal_printf("msgId inside pong is %d\n", msg.MSGID);
		//tmpMsgContent = (char*)"PING";
		msg.msgContent = (char*)"PING";
	}
	else {
		hal_printf("something wrong\n");
	}

	hal_printf("\n");
	//private_free(tmpMsgContent);

	/*Message_15_4_t** tempPtr = g_send_buffer.GetOldestPtr();
	hal_printf("start OMACTest::Receive\n");
	//if(g_OMAC.GetAddress() != (*tempPtr)->GetHeader()->src){
		hal_printf("OMACTest src is %u\n", (*tempPtr)->GetHeader()->src);
		hal_printf("OMACTest dest is %u\n", (*tempPtr)->GetHeader()->dest);
		UINT8* payload = (*tempPtr)->GetPayload();
		hal_printf("OMACTest payload is \n");
		for(int i = 1; i <= payloadSize; i++){
			hal_printf(" %d\n", payload[i-1]);
		}
		hal_printf("\n");*/
	//}
	//else {
		//hal_printf("OMACTest sender receiving its own msg??\n");
	//}

	/*Payload_t *rcvmsg = (Payload_t *) msg;
	if(rcvmsg->MSGID != RcvCount){
		//CPU_GPIO_SetPinState((GPIO_PIN) 0, TRUE);
	}
	RcvCount = rcvmsg->MSGID;*/
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
	static bool flag = false;
	//msg.data[10] = 10;
	for(int i = 1; i <= payloadSize; i++){
		msg.data[i-1] = i;
	}
	//char* tmpMsgContent = msg.msgContent;
	if(!flag){
		msg.MSGID = SendCount;
		//tmpMsgContent = (char*)"PING";
		msg.msgContent = (char*)"PING";
		flag = true;
	}

	UINT16 Neighbor2beFollowed = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed;
	if (g_NeighborTable.GetNeighborPtr(Neighbor2beFollowed) == NULL) {
		return FALSE;
	}

#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
#endif
	//Mac_Send(MacId, MAC_BROADCAST_ADDRESS, MFM_DATA, (void*) &msg.data, sizeof(Payload_t));
	if(setMsgReceivedAckFlag){
		//Mac_Send(MacId, Neighbor2beFollowed, MFM_DATA, (void*) &g_payload, sizeof(Payload_t));
		setMsgReceivedAckFlag = false;
	}
	hal_printf("msgId before sending is %d\n", msg.MSGID);
	bool ispcktScheduled = Mac_Send(MacId, Neighbor2beFollowed, MFM_DATA, (void*) &msg, sizeof(Payload_t));
	//if (ispcktScheduled == 0) {SendCount++;}
}

void OMACTest_Initialize(){
	g_OMACTest.Initialize();
	g_OMACTest.StartTest();
}


