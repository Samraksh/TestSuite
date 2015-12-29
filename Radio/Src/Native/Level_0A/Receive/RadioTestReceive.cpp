/*
 * TimesyncTest.cpp
 */

/*
 * RadioTestReceive.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: Ananth Muralidharan
 *      A simple test to measure the pseudo randomness of the PRNG
 */

#include "RadioTestReceive.h"


const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

RadioTestReceive g_RadioTestReceive;

#define DEBUG_RadioTest 1
#define TEST_0A_TIMER	12
#define TIMER_PERIOD 	0.5*ONESEC_IN_MSEC
#define Test_0A_Timer_Pin 1 //2


/*
 * Wait until a packet is received, before starting CCA
 */
void Test_0A_Timer_Handler(void * arg){
	g_RadioTestReceive.VerifyCCA();
}

void* RadioTest_ReceiveHandler (void* msg, UINT16 size){
	return g_RadioTestReceive.Receive(msg, size);
}

void RadioTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
	//g_RadioTestReceive.SendAck(msg,size,status);
}

/*
 * Verify CCA
 */
void RadioTestReceive::VerifyCCA()
{
	DeviceStatus DS = CPU_Radio_ClearChannelAssesment(this->radioName);
	if(DS == DS_Success){
		hal_printf("Channel is active!\n");
	}
	else if(DS == DS_Busy){
		hal_printf("CCA busy\n");
	}
	else if(DS == DS_Fail){
		hal_printf("CCA failed\n");
	}
	else{
		hal_printf("Unknown status\n");
	}
}

/*
 * As soon as a packet is received, send a packet in response and start timer which checks for energy in channel
 */
void* RadioTestReceive::Receive(void* tmpMsg, UINT16 size)
{
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	if(!initialPacketReceived){
		initialPacketReceived = true;
		SendPacket();
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Start(TEST_0A_TIMER);
		ASSERT(rm == TimerSupported);
	}
}

Message_15_4_t RadioTestReceive::CreatePacket()
{
	Message_15_4_t msg_carrier;
	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();

	header->length = sizeof(Payload_t) + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->src = CPU_Radio_GetAddress(this->radioName);

	return msg_carrier;
}

void RadioTestReceive::SendPacket()
{
	CPU_Radio_Send(this->radioName, &msg_carrier, (msg_carrier.GetHeader())->GetLength());
}

BOOL RadioTestReceive::StartTest()
{
	return TRUE;
}

BOOL RadioTestReceive::Initialize()
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Test_0A_Timer_Pin, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin, FALSE);

	initialPacketReceived = false;
	radioName = RF231RADIO;
	DeviceStatus status;
	Radio_Event_Handler.SetReceiveHandler(RadioTest_ReceiveHandler);
	if((status = CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, 1, 1)) != DS_Success){
		return status;
	}

	msg_carrier = CreatePacket();

	VirtTimer_Initialize();
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER, 0, TIMER_PERIOD*ONEMSEC_IN_USEC, FALSE, FALSE, Test_0A_Timer_Handler);
	ASSERT(rm == TimerSupported);

	return TRUE;
}

void RadioTest_Initialize()
{
	g_RadioTestReceive.Initialize();
	g_RadioTestReceive.StartTest();
}


