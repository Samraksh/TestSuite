/*
 * TimesyncTest.cpp
 */

/*
 * RadioTestSend.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: Ananth Muralidharan
 *      A test to verify the working of RF231's extended operating mode
 */

#include "RadioTestSend.h"


const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

RadioTestSend g_RadioTestSend;
Buffer_15_4_t g_send_buffer;
extern RF231Radio grf231Radio;

#define DEBUG_RadioTest 1
#define TEST_0A_TIMER1	10
#define TEST_0A_TIMER2	11
#define TIMER1_PERIOD 	5*ONESEC_IN_MSEC
#define TIMER2_PERIOD 	1000
#define Test_0A_Timer_Pin (GPIO_PIN)31 //2


/*
 * Keep sending a packet every x amount of time until a response is received
 */
void Test_0A_Timer1_Handler(void * arg){
	hal_printf("Test_0A_Timer1_Handler; Sending msgID: %d\n", g_RadioTestSend.msgID);
	g_RadioTestSend.SendPacket();
}

void Test_0A_Timer2_Handler(void * arg){
	hal_printf("Test_0A_Timer2_Handler; No h/w ack received\n");
}

void* RadioTest_ReceiveHandler (void* msg, UINT16 size){
	hal_printf("RadioTest_ReceiveHandler; ACK received\n");

	//If h/w ack is received, stop timer2 and increment msgID
	VirtTimer_Stop(TEST_0A_TIMER2);
	g_RadioTestSend.msgID++;
	return g_RadioTestSend.Receive(msg, size);
}

void RadioTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
	hal_printf("RadioTest_SendAckHandler; msg sent\n\n");
}

BOOL RadioTest_InterruptHandler(RadioInterrupt Interrupt, void *param){
	hal_printf("RadioTest_InterruptHandler\n");
}

/*
 * As soon as a response is received from another node, start the 2nd timer
 */
void* RadioTestSend::Receive(void* tmpMsg, UINT16 size)
{
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	/*if(!initialPacketReceived){
		initialPacketReceived = true;
		////VirtTimer_Start(TEST_0A_TIMER2);
	}*/
}

void RadioTestSend::CreatePacket()
{
	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();

	header->length = sizeof(Payload_t) + sizeof(IEEE802_15_4_Header_t);
	//header->fcf = (65 << 8); //(16640)
	//header->fcf |= 136; //(16776)
	//header->fcf = 9254;
	header->fcf = 9253;
	header->dsn = 97;
	//header->destpan = (34 << 8);
	//header->destpan |= 0;
	//header->destpan = 8704;
	header->destpan = 65535;
	header->src = CPU_Radio_GetAddress(this->radioName);
	header->dest = 6846;
	header->network = 138;
	//header->dest = 0xFFFF;
}

void RadioTestSend::SendPacket()
{
	Payload_t* data_msg = (Payload_t*)msg_carrier.GetPayload();
	msg.MSGID = msgID;
	for(int i = 1; i <= PAYLOAD_SIZE; i++){
		msg.data[i-1] = i;
	}
	*data_msg = msg;

	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin, TRUE);
	(Message_15_4_t*)grf231Radio.Send_TimeStamped(&msg_carrier, (msg_carrier.GetHeader())->GetLength(), HAL_Time_CurrentTicks());
	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin, FALSE);

	//After sending msg, start timer2 which waits for a h/w ack from sender.
	VirtTimer_Start(TEST_0A_TIMER2);
}

/*
 * Start TEST_0A_TIMER1 which keeps sending a packet with same msgID until a h/w ack is received
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

	//initialPacketReceived = false;
	radioName = RF231RADIO;
	DeviceStatus status;
	Radio_Event_Handler.SetRadioInterruptHandler(RadioTest_InterruptHandler);
	Radio_Event_Handler.SetSendAckHandler(RadioTest_SendAckHandler);
	Radio_Event_Handler.SetReceiveHandler(RadioTest_ReceiveHandler);
	status = grf231Radio.Initialize(&Radio_Event_Handler, this->radioName, 1);
	//grf231Radio.TurnOnRx();

	//Weird! VirtTimer_Initialize is needed to run this test in master branch, but not needed while running in OMAC_Master_Final branch
	VirtTimer_Initialize();
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, TIMER1_PERIOD*ONEMSEC_IN_USEC, FALSE, FALSE, Test_0A_Timer1_Handler);
	ASSERT(rm == TimerSupported);
	rm = VirtTimer_SetTimer(TEST_0A_TIMER2, 0, TIMER2_PERIOD*ONEMSEC_IN_USEC, TRUE, FALSE, Test_0A_Timer2_Handler);
	ASSERT(rm == TimerSupported);

	msgID = 1;
	CreatePacket();

	return TRUE;
}

void RadioTest_Initialize()
{
	g_RadioTestSend.Initialize();
	g_RadioTestSend.StartTest();
}


