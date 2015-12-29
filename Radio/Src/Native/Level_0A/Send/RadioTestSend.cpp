/*
 * TimesyncTest.cpp
 */

/*
 * RadioTestSend.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: Ananth Muralidharan
 *      A simple test to measure the pseudo randomness of the PRNG
 */

#include "RadioTestSend.h"


const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

RadioTestSend g_RadioTestSend;
//extern Time_Driver g_Time_Driver;

#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	10
#define TEST_0A_TIMER2	11
#define TIMER2_PERIOD 	0.5*ONESEC_IN_MSEC
#define Test_0A_Timer_Pin 1 //2


/*
 * Keep sending a packet every x amount of time until a response is received
 */
void Test_0A_Timer1_Handler(void * arg){
	while(!g_RadioTestSend.initialPacketReceived){
		g_RadioTestSend.SendPacket();
		/*DeviceStatus returnVal = CPU_Radio_TurnOnRx(g_RadioTestSend.radioName);
		if(returnVal != DS_Success){
			hal_printf("Could not put radio in Rx mode\n");
		}*/
		HAL_Time_Sleep_MicroSeconds(500);
		//g_Time_Driver.Sleep_uSec(500);
		//for(int i = 0; i < 10000; i++);
	}
}

/*
 * Alternates between sending packets and sleeping
 */
void Test_0A_Timer2_Handler(void * arg){
	static bool toggle = false;
	if(!toggle){
		toggle = true;
		for(int i = 0; i < 200; i++){
			g_RadioTestSend.SendPacket();
			HAL_Time_Sleep_MicroSeconds(500);
		}
	}
	else{
		toggle = false;
		HAL_Time_Sleep_MicroSeconds(400000);
	}
}

void* RadioTest_ReceiveHandler (void* msg, UINT16 size){
	return g_RadioTestSend.Receive(msg, size);
}

void RadioTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
	//g_RadioTestSend.SendAck(msg,size,status);
}

/*
 * As soon as a response is received from another node, start the 2nd timer
 */
void* RadioTestSend::Receive(void* tmpMsg, UINT16 size)
{
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	if(!initialPacketReceived){
		initialPacketReceived = true;
		VirtTimer_Start(TEST_0A_TIMER2);
	}
}

Message_15_4_t RadioTestSend::CreatePacket()
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

void RadioTestSend::SendPacket()
{
	Message_15_4_t* returnMsg = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(this->radioName, &msg_carrier, (msg_carrier.GetHeader())->GetLength(), HAL_Time_CurrentTicks());
}

/*
 * Start TEST_0A_TIMER1 which keeps sending a packet until a response is received
 */
BOOL RadioTestSend::StartTest()
{
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Start(TEST_0A_TIMER1);
	ASSERT(rm == TimerSupported);

	return TRUE;
}

BOOL RadioTestSend::Initialize()
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Test_0A_Timer_Pin, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin, FALSE);

	initialPacketReceived = false;
	radioName = RF231RADIO;
	DeviceStatus status;
	Radio_Event_Handler.SetReceiveHandler(RadioTest_ReceiveHandler);
	if((status = CPU_Radio_Initialize(&Radio_Event_Handler, this->radioName, 1, 1)) != DS_Success){
		SOFT_BREAKPOINT();
		return status;
	}
	if((status = CPU_Radio_TurnOnRx(this->radioName)) != DS_Success) {
		SOFT_BREAKPOINT();
		return status;
	}

	msg_carrier = CreatePacket();

	VirtTimer_Initialize();
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, 0, TRUE, FALSE, Test_0A_Timer1_Handler);
	ASSERT(rm == TimerSupported);
	rm = VirtTimer_SetTimer(TEST_0A_TIMER2, 0, TIMER2_PERIOD*ONEMSEC_IN_USEC, FALSE, FALSE, Test_0A_Timer2_Handler);
	ASSERT(rm == TimerSupported);

	return TRUE;
}

void RadioTest_Initialize()
{
	g_RadioTestSend.Initialize();
	g_RadioTestSend.StartTest();
}


