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
extern RF231Radio grf231Radio;

#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	7
#define TEST_0A_TIMER2	11
#define TIMER2_PERIOD 	1*ONESEC_IN_MSEC
#define Test_0A_Timer_Pin (GPIO_PIN)31 //2



static void RadioTestSend:: TxDone( bool success){

}
static void RadioTestSend:: PacketDetected(){

}
static void RadioTestSend:: RxDone (uint8_t *payload, uint16_t size ){

}
static void RadioTestSend:: CadDone ( bool channelActivityDetected ){

}
static void RadioTestSend:: DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer ){

}

static void Test_0A_Timer1_Handler(void * arg){
		g_RadioTestSend.ScheduleSendPacket();
}




void RadioTestSend::ScheduleSendPacket()
{
}

/*
 * Start TEST_0A_TIMER1 which keeps sending a packet until a response is received
 */
BOOL RadioTestSend::StartTest()
{
	ClockIdentifier_t ClockIdentifier = 4;
	TimeVariable_t PacketTransmissionTime = HAL_TIME_CURTICKS(ClockIdentifier) + 1000;

	radio->RequestSendAtTimeInstanst(msg, sizeof(Payload_t), PacketTransmissionTime, ClockIdentifier);

	return TRUE;
}

BOOL RadioTestSend::Initialize()
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Test_0A_Timer_Pin, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin, FALSE);

	radio = &grf231Radio;
	radio->Initialize(radio_events);


//	VirtualTimerReturnMessage rm;
//	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, 1000000, FALSE, FALSE, Test_0A_Timer1_Handler);
//	ASSERT(rm == TimerSupported);

	return TRUE;
}

void RadioTest_Initialize()
{
	g_RadioTestSend.Initialize();
	g_RadioTestSend.StartTest();
}


