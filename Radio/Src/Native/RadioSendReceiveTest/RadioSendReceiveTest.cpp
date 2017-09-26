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
		hal_printf("TestObject_t::TxDone SUCCESS %d number_of_bytes_in_buffer = %d  \r\n" , success);
		g_TestObject_ptr->m_PacketSentTxDone = true;
		CPU_GPIO_SetPinState( LED_RED, LED_OFF_STATE );
		CPU_GPIO_SetPinState( LED_GREEN, LED_ON_STATE );
	}
	else{
		hal_printf("TestObject_t::TxDone FAIL %d number_of_bytes_in_buffer = %d  \r\n" , success);
		g_TestObject_ptr->m_PacketSentTxDone = false;
	}
	gsx1276radio_ptr->StartListenning();
}
void TestObject_t:: PacketDetected(){
	CPU_GPIO_SetPinState( LED_BLUE, LED_ON_STATE );
	CPU_GPIO_SetPinState( LED_BLUE, LED_OFF_STATE );
}
void TestObject_t:: RxDone (uint8_t *payload, uint16_t size ){
	g_TestObject_ptr->ReceivePacket(payload, size);
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
	CPU_GPIO_SetPinState( LED_RED, LED_OFF_STATE );
	CPU_GPIO_SetPinState( LED_GREEN, LED_OFF_STATE );
	CPU_GPIO_SetPinState( LED_BLUE, LED_OFF_STATE );
	g_TestObject_ptr->SendPacket();
	hal_printf("*** RadioFIFOTest::End Test  \r\n");

	VirtualTimerReturnMessage rm;
	if(rm == TimerSupported) {do { rm = VirtTimer_Start(TEST_0A_TIMER1); } while(rm != TimerSupported);}
}


void TestObject_t::IncrementBuffers(){
	msg_written.SetMsg(msg_written.array[1] + 1); //Initialize buffers
	msg_read.SetMsg(msg_written.array[1] + 1); //Initialize buffers
}

void TestObject_t::ReceivePacket(uint8_t *payload, uint16_t size ){

	bool rv;
	if(size > BYTELENGTHOFNESSAGE) rv = false;
	else{
		memcpy(&msg_read.array[1], payload, size);
		rv = msg_read.CheckMsgConsistency();
	}

	if(rv) {
		CPU_GPIO_SetPinState( LED_BLUE, LED_ON_STATE );
		hal_printf("SUCCESS: Received packet is consistent and has an ID of %llu", msg_read.array[1] );
	}
	else{
		CPU_GPIO_SetPinState( LED_RED, LED_ON_STATE );
		hal_printf("FAIL: Received packet is inconsistent");
	}
}


void TestObject_t::SendPacket()
{
	CPU_GPIO_SetPinState( LED_RED, LED_ON_STATE );
	gsx1276radio_ptr->Send(static_cast<void*>(&msg_written.array[1]), BYTELENGTHOFNESSAGE);
	return;
}





bool TestObject_t::Initialize()
{
	CPU_GPIO_EnableOutputPin(LED_RED, LED_ON_STATE);
	CPU_GPIO_SetPinState( LED_RED, LED_OFF_STATE );
	CPU_GPIO_EnableOutputPin( LED_GREEN, LED_ON_STATE);
	CPU_GPIO_SetPinState( LED_GREEN, LED_OFF_STATE );
	CPU_GPIO_EnableOutputPin( LED_BLUE, LED_ON_STATE);
	CPU_GPIO_SetPinState( LED_BLUE, LED_OFF_STATE );

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

	if(rm == TimerSupported) {do { rm = VirtTimer_Start(TEST_0A_TIMER1); } while(rm != TimerSupported);}

	return true;
}



void Test_InitializeAndRun()
{
	static TestObject_t g_TestObject;
	NSRadioSendReceiveTest::g_TestObject_ptr = &g_TestObject;
	NSRadioSendReceiveTest::g_TestObject_ptr->Initialize();

}

}  //Endof namespace

