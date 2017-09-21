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

#include "RadioFIFOTest.h"
#include <Samraksh/VirtualTimer.h>

const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

TestObject_t* g_TestObject_ptr;
extern SamrakshRadio_I* gsx1276radio_ptr;
extern Samraksh_SX1276Wrapper::SX1276M1BxASWrapper* g_SX1276M1BxASWrapper_ptr;



#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	7
#define TEST_0A_TIMER2	11
#define TIMER2_PERIOD 	40*ONESEC_IN_MSEC*ONEMSEC_IN_USEC
#define Test_0A_Timer_Pin (GPIO_PIN)25 //2



void TestObject_t:: TxDone( bool success){

//	if(success){
//		hal_printf("TestObject_t::TxDone success %d number_of_bytes_in_buffer = %d  \r\n" , success);
//	}
//	else{
//		hal_printf("TestObject_t::TxDone success %d number_of_bytes_in_buffer = %d  \r\n" , success);
//	}
////	g_TestObject_t.StartTest();
}
void TestObject_t:: PacketDetected(){

}
void TestObject_t:: RxDone (uint8_t *payload, uint16_t size ){

}
void TestObject_t:: CadDone ( bool channelActivityDetected ){

}
void TestObject_t:: DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer ){
//	hal_printf("TestObject_t::DataStatusCallback success %d number_of_bytes_in_buffer = %d  \r\n" , success, number_of_bytes_in_buffer);
//	if(success){
//
//	}
//	else{
//		g_TestObject_t.delay = g_TestObject_t.delay*2;
////		g_TestObject_t.StartTest();
//	}
}

void Test_0A_Timer1_Handler(void * arg){
	hal_printf("*** RadioFIFOTest::Start Test \r\n");


	g_TestObject_ptr->IncrementBuffers();
	g_TestObject_ptr->WriteToBuffer();
	g_TestObject_ptr->ReadFromBuffer();
	g_TestObject_ptr->CompareBuffers();
	hal_printf("*** RadioFIFOTest::End Test  \r\n");
}



void TestObject_t::IncrementBuffers(){
	msg_written.SetMsg(msg_written.array[1] + 1); //Initialize buffers
	msg_read.SetMsg(msg_written.array[1] + 1); //Initialize buffers
}

bool TestObject_t::ReadFromBuffer()
{
	g_SX1276M1BxASWrapper_ptr-> ReadFromTxBuffer(
			reinterpret_cast<uint8_t*>(&msg_read.array[1])
			, BYTELENGTHOFNESSAGE
			);

	return TRUE;
}

bool TestObject_t::WriteToBuffer()
{
	g_SX1276M1BxASWrapper_ptr->AddToTxBuffer(
			reinterpret_cast<uint8_t*>(&(msg_written.array[1]))
			, BYTELENGTHOFNESSAGE
			);

	return TRUE;
}

bool TestObject_t::CompareBuffers(){
	if(msg_written == msg_read){
		hal_printf("SUCCESS");
	}
	else{
		hal_printf("FAIL");
	}
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

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, TIMER2_PERIOD, FALSE, FALSE, Test_0A_Timer1_Handler);

	rm = VirtTimer_Start(TEST_0A_TIMER1);



	return TRUE;
}


void Test_InitializeAndRun()
{
	static TestObject_t g_TestObject;
	g_TestObject_ptr = &g_TestObject;
	g_TestObject_ptr->Initialize();

}



