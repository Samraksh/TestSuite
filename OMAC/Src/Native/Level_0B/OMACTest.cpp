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

const UINT16 ONEMSEC = 1000;
const UINT16 ONEUSEC = 1000;

OMACTest g_OMACTest;
extern NeighborTable g_NeighborTable;
extern OMACScheduler g_omac_scheduler;
extern OMACType g_OMAC;
extern UINT16 MF_NODE_ID;
extern Buffer_15_4_t g_send_buffer;

void Timer_32_Handler(void * arg){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
#endif
	g_OMACTest.Send();
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
#endif
}

// Typedef defining the signature of the receive function
//void ReceiveHandler (void *msg, UINT16 Size, UINT16 Src, BOOL Unicast, UINT8 RSSI, UINT8 LinkQuality){
void OMACTest_ReceiveHandler (UINT16 size){
	return g_OMACTest.Receive(size);
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

	VirtTimer_SetTimer(32, 0, 10*ONEMSEC*ONEUSEC, FALSE, FALSE, Timer_32_Handler); //period (3rd argument) is in micro seconds
	return TRUE;
}

BOOL OMACTest::StartTest(){
	msg.MSGID = 0;
	SendCount = 0;
	RcvCount = 0;
	//while(1){
		VirtTimer_Start(32);
	//}

	return TRUE;
}

void OMACTest::Receive(UINT16 size){
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN)30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)30, FALSE);
#endif
	Message_15_4_t** tempPtr = g_send_buffer.GetOldestPtr();
	hal_printf("start OMACTest::Receive\n");
	//if(g_OMAC.GetAddress() != (*tempPtr)->GetHeader()->src){
		hal_printf("OMACTest src is %u\n", (*tempPtr)->GetHeader()->src);
		hal_printf("OMACTest dest is %u\n", (*tempPtr)->GetHeader()->dest);
		UINT8* payload = (*tempPtr)->GetPayload();
		hal_printf("OMACTest payload is \n");
		for(int i = 1; i <= 10; i++){
			hal_printf(" %d\n", payload[i-1]);
		}
		hal_printf("\n");
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
	msg.MSGID = SendCount;
	//msg.data[10] = 10;
	for(int i = 1; i <= 10; i++){
		msg.data[i-1] = i;
	}

	UINT16 Nbr2beFollowed = g_omac_scheduler.m_TimeSyncHandler.Neighbor2beFollowed;
	if (g_NeighborTable.GetNeighborPtr(dest) == NULL) {
		return FALSE;
	}
#ifdef DEBUG_OMACTest
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
#endif
	//Mac_Send(MacId, MAC_BROADCAST_ADDRESS, MFM_DATA, (void*) &msg.data, sizeof(Payload_t));
	bool ispcktScheduled = Mac_Send(MacId, Nbr2beFollowed, MFM_DATA, (void*) &msg.data, sizeof(Payload_t));
	if (ispcktScheduled) {SendCount++;}
}

void OMACTest_Initialize(){
	g_OMACTest.Initialize();
	g_OMACTest.StartTest();
}


