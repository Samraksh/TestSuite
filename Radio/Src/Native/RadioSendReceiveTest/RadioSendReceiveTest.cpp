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

#include "RadioSendReceiveTest.h"
#include <Samraksh/VirtualTimer.h>


extern SamrakshRadio_I* gsx1276radio_ptr;

namespace NSRadioSendReceiveTest{

TestObject_t* g_TestObject_ptr;



const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;



#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	7
#define TEST_0A_TIMER2	11
#define TIMER2_PERIOD 	40*ONESEC_IN_MSEC*ONEMSEC_IN_USEC
#define Test_0A_Timer_Pin (GPIO_PIN)25 //2



void TestObject_t:: TxDone( bool success){
	if(success){
		hal_printf("TestObject_t::TxDone success %d number_of_bytes_in_buffer = %d  \r\n" , success);
		g_TestObject_ptr->m_PacketSentTxDone = true;
	}
	else{
		hal_printf("TestObject_t::TxDone success %d number_of_bytes_in_buffer = %d  \r\n" , success);
		g_TestObject_ptr->m_PacketSentTxDone = false;
	}
	gsx1276radio_ptr->StartListenning();
}
void TestObject_t:: PacketDetected(){

}
void TestObject_t:: RxDone (uint8_t *payload, uint16_t size ){
	g_TestObject_ptr->ReceivePacket(payload, size);
	g_TestObject_ptr->ComparePackets();
}
void TestObject_t:: CadDone ( bool channelActivityDetected ){

}
void TestObject_t:: DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer ){
	hal_printf("TestObject_t::DataStatusCallback success %d number_of_bytes_in_buffer = %d  \r\n" , success, number_of_bytes_in_buffer);
	g_TestObject_ptr->m_PacketSentAccepted = success;
	if(!success){
		gsx1276radio_ptr->StartListenning();
	}
}

void Test_0A_Timer1_Handler(void * arg){
	hal_printf("*** RadioFIFOTest::Start Test \r\n");
	g_TestObject_ptr->SendPacket();
	hal_printf("*** RadioFIFOTest::End Test  \r\n");
}



void TestObject_t::ReceivePacket(uint8_t *payload, uint16_t size ){
	for(uint16_t s = 1; s < size+1; ++s) {
			msg_read.array[s] = payload[s-1];
	}
}

void TestObject_t::SendPacket()
{
	gsx1276radio_ptr->Send(static_cast<void*>(&msg_written.array[1]), BYTELENGTHOFNESSAGE);
	return;
}

bool TestObject_t::ComparePackets(){

	if(msg_written == msg_read){
		hal_printf("SUCCESS");
		m_RxDone = true;

	}
	else{
		hal_printf("FAIL");
		m_RxDone = false;
	}
	return m_RxDone;
}

bool TestObject_t::Initialize()
{
	radio_events.TxDone = TxDone;
	radio_events.PacketDetected = PacketDetected;
	radio_events.RxDone = RxDone;
	radio_events.CadDone = CadDone;
	radio_events.DataStatusCallback = DataStatusCallback;

	hal_printf("*** RadioFIFOTest::Initialize  \r\n");

	EMOTE_SX1276_LORA::Samraksh_SX1276_hal_Init();
	DeviceStatus ds = gsx1276radio_ptr->Initialize(radio_events);

	msg_written.SetMsg(MSG_EMBED); //Initialize buffers
	msg_read.SetMsg(MSG_EMBED+1);

	m_RxDone = false;
	m_PacketDetected = false;

	m_PacketSentAccepted = false;
	m_PacketSentTxDone = false;

	gsx1276radio_ptr->StartListenning();
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, TIMER2_PERIOD, FALSE, FALSE, Test_0A_Timer1_Handler);

	rm = VirtTimer_Start(TEST_0A_TIMER1);

	return TRUE;
}



void Test_InitializeAndRun()
{
	static TestObject_t g_TestObject;
	NSRadioSendReceiveTest::g_TestObject_ptr = &g_TestObject;
	NSRadioSendReceiveTest::g_TestObject_ptr->Initialize();

}

}  //Endof namespace
