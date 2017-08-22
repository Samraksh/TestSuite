/*
 * TimesyncTest.cpp
 */

/*
 * TestObject_t.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: Ananth Muralidharan
 *      A simple test to measure the pseudo randomness of the PRNG
 */

#include "RequestSendAtTimeInstantTest.h"


const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

TestObject_t g_TestObject_t;
extern EMOTE_SX1276_LORA::Samraksh_SX1276_hal gsx1276radio;
EMOTE_SX1276_LORA::Samraksh_SX1276_hal gsx1276radio2;


#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	7
#define TEST_0A_TIMER2	11
#define TIMER2_PERIOD 	1*ONESEC_IN_MSEC
#define Test_0A_Timer_Pin (GPIO_PIN)31 //2



void TestObject_t:: TxDone( bool success){

}
void TestObject_t:: PacketDetected(){

}
void TestObject_t:: RxDone (uint8_t *payload, uint16_t size ){

}
void TestObject_t:: CadDone ( bool channelActivityDetected ){

}
void TestObject_t:: DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer ){

}

static void Test_0A_Timer1_Handler(void * arg){
		g_TestObject_t.ScheduleSendPacket();
}




void TestObject_t::ScheduleSendPacket()
{
}

/*
 * Start TEST_0A_TIMER1 which keeps sending a packet until a response is received
 */
BOOL TestObject_t::StartTest()
{
	SamrakshRadio_I::ClockIdentifier_t ClockIdentifier = 4;
	SamrakshRadio_I::TimeVariable_t PacketTransmissionTime = HAL_Time_CurrentTicks() + 1000;

	radio->RequestSendAtTimeInstanst(static_cast<void*>(&msg), sizeof(Payload_t), PacketTransmissionTime, ClockIdentifier);

	return TRUE;
}

BOOL TestObject_t::Initialize()
{
	CPU_GPIO_EnableOutputPin(Test_0A_Timer_Pin, TRUE);
	CPU_GPIO_SetPinState(Test_0A_Timer_Pin, FALSE);

	radio_events = {TxDone, PacketDetected, RxDone, CadDone, DataStatusCallback};


	radio = dynamic_cast<SamrakshRadio_I*> (&gsx1276radio2);
	radio->Initialize(radio_events);


//	VirtualTimerReturnMessage rm;
//	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, 1000000, FALSE, FALSE, Test_0A_Timer1_Handler);
//	ASSERT(rm == TimerSupported);

	return TRUE;
}

void Test_InitializeAndRun()
{
	g_TestObject_t.Initialize();
	g_TestObject_t.StartTest();
}


