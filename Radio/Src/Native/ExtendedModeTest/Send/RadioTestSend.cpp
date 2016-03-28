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
#define TIMER1_PERIOD 	10*ONESEC_IN_MSEC
#define TIMER2_PERIOD 	7
#define Transmission_Pin (GPIO_PIN)31 //2
#define ACK_Pin (GPIO_PIN)1 //2

/*
 * Keep sending a packet every x amount of time until a response is received
 */
void Test_0A_Timer1_Handler(void * arg){
	//If it is a new attempt at sending a msg or the final, increment the msgID
	if(g_RadioTestSend.currentTry == 0 || g_RadioTestSend.currentTry >= g_RadioTestSend.retryLimit){
		g_RadioTestSend.msgID++;
		if(g_RadioTestSend.currentTry >= g_RadioTestSend.retryLimit){
			g_RadioTestSend.currentTry = 0;
		}
	}

	//Keep sending msgs until currentTry reaches retryLimit
	if(g_RadioTestSend.currentTry < g_RadioTestSend.retryLimit){
		hal_printf("Test_0A_Timer1_Handler; Sending msgID: %d; current attempt: %d\n", g_RadioTestSend.msgID, g_RadioTestSend.currentTry);
		g_RadioTestSend.SendPacket();
	}
}

void Test_0A_Timer2_Handler(void * arg){
	hal_printf("Test_0A_Timer2_Handler; No h/w ack received\n");
	g_RadioTestSend.currentTry++;
}

void* RadioTest_ReceiveHandler (void* msg, UINT16 size){
	CPU_GPIO_SetPinState((GPIO_PIN) ACK_Pin, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) ACK_Pin, FALSE);
	hal_printf("RadioTest_ReceiveHandler; ACK received\n");

	//If h/w ack is received, stop timer2 and increment msgID
	VirtTimer_Stop(TEST_0A_TIMER2);
	//Ack received, make currentTry 0 for the next send
	g_RadioTestSend.currentTry = 0;
	return g_RadioTestSend.Receive(msg, size);
}

void RadioTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
	hal_printf("RadioTest_SendAckHandler; msg sent; status: %d\n", status);
	if(radioAckStatus == TRAC_STATUS_SUCCESS || radioAckStatus == TRAC_STATUS_SUCCESS_DATA_PENDING){
		hal_printf("RadioTest_SendAckHandler; ACK received; radioAckStatus: %d\n\n", radioAckStatus);
	}
	else if(radioAckStatus == TRAC_STATUS_NO_ACK || radioAckStatus == TRAC_STATUS_FAIL_TO_SEND){
		hal_printf("RadioTest_SendAckHandler; NO ACK received; radioAckStatus: %d\n\n", radioAckStatus);
	}
	else{
		hal_printf("RadioTest_SendAckHandler; radioAckStatus: %d\n\n", radioAckStatus);
	}
}

BOOL RadioTest_InterruptHandler(RadioInterrupt Interrupt, void *param){
	//hal_printf("RadioTest_InterruptHandler\n");
}

/*
 * As soon as a response is received from another node, start the 2nd timer
 */
void* RadioTestSend::Receive(void* tmpMsg, UINT16 size)
{
	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	IEEE802_15_4_Header_t *header = (IEEE802_15_4_Header_t*)rcvdMsg->GetHeader();
	hal_printf("header->dsn: %d\n\n", header->dsn);
	//Payload_t* payload = (Payload_t*)rcvdMsg->GetPayload();
	//hal_printf("Received msgID: %d\n\n", payload->MSGID);
	/*if(!initialPacketReceived){
		initialPacketReceived = true;
		////VirtTimer_Start(TEST_0A_TIMER2);
	}*/
}

void RadioTestSend::CreatePacket()
{
	static UINT8 seqNumber = 0;
	UINT8 finalSeqNumber = 0;
	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();

	//header->length = sizeof(Payload_t) + sizeof(IEEE802_15_4_Header_t);
	//header->fcf = (65 << 8); //(16640)
	//header->fcf |= 136; //(16776)
	//header->fcf = 9254;
	//header->destpan = (34 << 8);
	//header->destpan |= 0;
	//header->frameLength = 17;
	header->fcf.fcfWordValue = FCF_WORD_VALUE;
	finalSeqNumber = myAddress ^ 0xAA;
	finalSeqNumber += ((myAddress >> 8) ^ 0x55);
	finalSeqNumber += seqNumber;
	if(finalSeqNumber == OMAC_DISCO_SEQ_NUMBER){
		finalSeqNumber += 1;
	}
	header->dsn = finalSeqNumber;
	//header->srcpan = 0x0;
	header->src = myAddress;
	/*if(CPU_Radio_GetAddress(this->radioName) == 6846){
		header->dest = 0x0DB1;
	}
	else{
		header->dest = 0x1ABE;
	}*/
	//header->dest = 6846;
	header->destpan = DEST_PAN_ID;
	header->dest = 6846;

	Payload_t* data_msg = (Payload_t*)msg_carrier.GetPayload();
	msg.MSGID = msgID;
	for(int i = 1; i <= PAYLOAD_SIZE; i++){
		msg.data[i-1] = i;
	}
	*data_msg = msg;
	seqNumber++;

	/*udp_header.srcPort = 17754;
	udp_header.destPort = 17754;
	udp_header.checkSum = 0;
	udp_header.length = sizeof(ZEP_Header)+sizeof(IEEE802_15_4_Header_t);
	memcpy(udp_header.payload, &(zepHeader), sizeof(ZEP_Header));
	memcpy(udp_header.payload+sizeof(ZEP_Header), msg_carrier.GetHeader(), sizeof(IEEE802_15_4_Header_t));

	zepHeader.hf_zep_channel_id = 0;
	zepHeader.hf_zep_version = 1;
	zepHeader.hf_zep_device_id = 1;
	zepHeader.hf_zep_lqi = 0;
	zepHeader.hf_zep_lqi_mode = 0;
	//zepHeader.hf_zep_reserved_field = 0;
	zepHeader.hf_zep_ieee_length = sizeof(IEEE802_15_4_Header_t);*/


	/*IEEE802_15_4_Footer* footer = msg_carrier.GetFooter();
	footer->FCS = 0xAAAA;*/
	//header->fcf = 9253;
	//header->dsn = 97;
	//header->destpan = (34 << 8);
	//header->destpan |= 0;
	//header->destpan = 8704;
	//header->destpan = 65535;

	//header->dest = 6846;
	//header->network = 138;
	//header->dest = 0xFFFF;
}

void RadioTestSend::SendPacket()
{
	static UINT8 seqNumber = 0;

	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();
	header->fcf.fcfWordValue = FCF_WORD_VALUE;
	finalSeqNumber = myAddress ^ 0xAA;
	finalSeqNumber += ((myAddress >> 8) ^ 0x55);
	finalSeqNumber += seqNumber;
	if(finalSeqNumber == OMAC_DISCO_SEQ_NUMBER){
		finalSeqNumber += 1;
	}
	header->dsn = finalSeqNumber;
	header->src = myAddress;
	header->destpan = DEST_PAN_ID;
	header->dest = 6846;

	Payload_t* data_msg = (Payload_t*)msg_carrier.GetPayload();
	msg.MSGID = msgID;
	for(int i = 1; i <= PAYLOAD_SIZE; i++){
		msg.data[i-1] = i;
	}
	*data_msg = msg;
	seqNumber++;

	/*UINT8 msgBuffer[sizeof(IEEE802_15_4_Header_t)+sizeof(Payload_t)-1];
	//FCF
	msgBuffer[0] = 0x61; msgBuffer[1] = 0x88;
	//Seq number
	msgBuffer[2] = 0x01;
	//DestPan
	msgBuffer[3] = 0x00; msgBuffer[4] = 0x00;
	//Dest
	msgBuffer[5] = 0xBE; msgBuffer[6] = 0x1A;
	//Src
	msgBuffer[7] = 0xBA; msgBuffer[8] = 0x7B;
	//Msg payload
	msgBuffer[9] = 0x01; msgBuffer[10] = 0x00;
	msgBuffer[11] = 0x01; msgBuffer[12] = 0x02; msgBuffer[13] = 0x03; msgBuffer[14] = 0x04; msgBuffer[15] = 0x05; msgBuffer[16] = 0x00;*/
	//memcpy(msgBuffer, &msg_carrier, sizeof(IEEE802_15_4_Header_t)+sizeof(Payload_t));

	CPU_GPIO_SetPinState((GPIO_PIN) Transmission_Pin, TRUE);
	//(Message_15_4_t*)grf231Radio.Send_TimeStamped(&msg_carrier, (msg_carrier.GetHeader())->GetLength(), HAL_Time_CurrentTicks());
	(Message_15_4_t*)grf231Radio.Send_TimeStamped(&msg_carrier, sizeof(IEEE802_15_4_Header_t)+sizeof(Payload_t), HAL_Time_CurrentTicks());
	//(Message_15_4_t*)grf231Radio.Send_TimeStamped(msgBuffer, sizeof(IEEE802_15_4_Header_t)+sizeof(Payload_t)-1, HAL_Time_CurrentTicks());
	//(Message_15_4_t*)grf231Radio.Send_TimeStamped(&udp_header, 112, HAL_Time_CurrentTicks());
	//(Message_15_4_t*)grf231Radio.Send(&msg_carrier, 70);
	CPU_GPIO_SetPinState((GPIO_PIN) Transmission_Pin, FALSE);

	//After sending msg, start timer2 which waits for a h/w ack from sender.
	VirtTimer_Start(TEST_0A_TIMER2);
}

/*
 * Start TEST_0A_TIMER1 which keeps sending a packet with same msgID until a h/w ack is received
 */
BOOL RadioTestSend::StartTest()
{
	VirtualTimerReturnMessage rm;
	VirtTimer_Stop(TEST_0A_TIMER2);
	rm = VirtTimer_Start(TEST_0A_TIMER1);
	ASSERT(rm == TimerSupported);

	return TRUE;
}

BOOL RadioTestSend::Initialize()
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Transmission_Pin, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) Transmission_Pin, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) ACK_Pin, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) ACK_Pin, FALSE);

	//initialPacketReceived = false;
	finalSeqNumber = myAddress ^ 0xAA;
	finalSeqNumber += ((myAddress >> 8) ^ 0x55);
	radioName = RF231RADIO;
	DeviceStatus status;
	Radio_Event_Handler.SetRadioInterruptHandler(RadioTest_InterruptHandler);
	Radio_Event_Handler.SetSendAckHandler(RadioTest_SendAckHandler);
	Radio_Event_Handler.SetReceiveHandler(RadioTest_ReceiveHandler);
	status = grf231Radio.Initialize(&Radio_Event_Handler, this->radioName, 1);
	myAddress = grf231Radio.GetAddress();
	grf231Radio.TurnOnRx();

	hal_printf("My address is: %d; I am in send mode\n", myAddress);

	//Weird! VirtTimer_Initialize is needed to run this test in master branch, but not needed while running in OMAC_Master_Final branch
	//VirtTimer_Initialize();
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER1, 0, TIMER1_PERIOD*ONEMSEC_IN_USEC, FALSE, FALSE, Test_0A_Timer1_Handler);
	ASSERT(rm == TimerSupported);
	/*rm = VirtTimer_SetTimer(TEST_0A_TIMER2, 0, TIMER2_PERIOD*ONEMSEC_IN_USEC, TRUE, FALSE, Test_0A_Timer2_Handler);
	ASSERT(rm == TimerSupported);*/

	msgID = 0;
	retryLimit = 2;
	CreatePacket();

	return TRUE;
}

void RadioTest_Initialize()
{
	g_RadioTestSend.Initialize();
	g_RadioTestSend.StartTest();
}

