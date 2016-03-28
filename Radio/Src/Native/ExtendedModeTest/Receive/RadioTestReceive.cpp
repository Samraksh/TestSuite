/*
 * TimesyncTest.cpp
 */

/*
 * RadioTestReceive.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: Ananth Muralidharan
 *      A test to verify the working of RF231's extended operating mode
 */

#include "RadioTestReceive.h"


const UINT16 ONESEC_IN_MSEC = 1000;
const UINT16 ONEMSEC_IN_USEC = 1000;

RadioTestReceive g_RadioTestReceive;
extern RF231Radio grf231Radio;

#define DEBUG_RadioTest 1
#define TEST_0A_TIMER	10
#define TIMER_PERIOD 	2
#define Test_0A_Timer_Pin_30 (GPIO_PIN)30 //2
#define Test_0A_Timer_Pin_31 (GPIO_PIN)31 //2

#define testTimeout 60000	//Keep in sync with Parameters.h

UINT8 RadioTestReceive::nodeCount = 0;
UINT64 RadioTestReceive::startTicks = 0;
UINT64 RadioTestReceive::endTicks = 0;


/*
 * Wait until a packet is received, before starting CCA
 */
void Test_0A_Timer_Handler(void * arg){
	/*while(true){
		g_RadioTestReceive.VerifyCCA();
	}*/
}

void* RadioTest_ReceiveHandler (void* msg, UINT16 size){
	//hal_printf("RadioTest_ReceiveHandler; msg received\n");
	return g_RadioTestReceive.Receive(msg, size);
}

void RadioTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
	hal_printf("RadioTest_SendAckHandler; msg sent\n");
}

BOOL RadioTest_InterruptHandler(RadioInterrupt Interrupt, void *param){
	//hal_printf("RadioTest_InterruptHandler\n");
}

BOOL RadioTestReceive::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	while(true){
		hal_printf("result=%s\n", (result) ? "PASS":"FAIL");
		hal_printf("accuracy=%d\n", accuracy);
		hal_printf("resultParameter1=%s\n", resultParameter1);
		hal_printf("resultParameter2=%s\n", resultParameter2);
		hal_printf("resultParameter3=null\n");
		hal_printf("resultParameter4=null\n");
		hal_printf("resultParameter5=null\n");
		HAL_Time_Sleep_MicroSeconds(1000000);
	}
	return true;
}

/*
 * Verify CCA
 */
void RadioTestReceive::VerifyCCA()
{
	DeviceStatus DS = grf231Radio.ClearChannelAssesment();
	if(DS == DS_Fail){
		CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin_31, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin_31, FALSE);
		//hal_printf("Channel is active!\n");
	}
	else if(DS == DS_Success){
		CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin_30, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin_30, FALSE);
		//hal_printf("CCA failed\n");
	}
}

int RadioTestReceive::LookupNodeID(UINT16 nodeID){
	int ret = -1;
	for(int i = 0; i < MAX_NEIGHBORS; i++){
		if(newNode[i].nodeStats.nodeID == nodeID){
			ret = i;
			break;
		}
	}
	return ret;
}

/*
 *
 */
void* RadioTestReceive::Receive(void* tmpMsg, UINT16 size)
{
	if(receiveCount == 0){
		startTicks = HAL_Time_CurrentTicks();
	}
	receiveCount++;

	Message_15_4_t* rcvdMsg = (Message_15_4_t*)tmpMsg;
	IEEE802_15_4_Header_t *header = (IEEE802_15_4_Header_t*)rcvdMsg->GetHeader();
	Payload_t* payload = (Payload_t*)rcvdMsg->GetPayload();
	hal_printf("Received msgID: %d\n\n", payload->MSGID);

	UINT16 srcID = header->src;
	int nodeIndex = LookupNodeID(srcID);
	if(nodeIndex == -1){
		newNode[nodeCount].nodeStats.nodeID = srcID;
		newNode[nodeCount].nodeStats.lastMSGID = payload->MSGID;
		newNode[nodeCount].nodeStats.totalRecvCount = 1;
		nodeCount++;
	}
	else{
		newNode[nodeIndex].nodeStats.lastMSGID = payload->MSGID;
		newNode[nodeIndex].nodeStats.totalRecvCount += 1;
	}
	endTicks = HAL_Time_CurrentTicks();
	UINT64 duration = endTicks - startTicks;
	UINT64 durationMilliSec = CPU_TicksToMicroseconds(duration, 1);
	durationMilliSec = durationMilliSec/1000;
	if(durationMilliSec > testTimeout){
		ShowStats();
	}
	/*if(!initialPacketReceived){
		initialPacketReceived = true;
		SendPacket();
		VirtualTimerReturnMessage rm;
		rm = VirtTimer_Start(TEST_0A_TIMER);
		ASSERT(rm == TimerSupported);
	}*/
}

void RadioTestReceive::ShowStats(){
	for(int i = 0; i < nodeCount; i++){
		if(newNode[i].nodeStats.lastMSGID != newNode[i].nodeStats.totalRecvCount){
			DisplayStats(false, "ERROR: send receive test (Level_0B) failed", NULL, 0);
		}
	}
	DisplayStats(true, "SUCCESS: send receive test (Level_0B) succeeded", NULL, 0);
}

Message_15_4_t RadioTestReceive::CreatePacket()
{
	/*Message_15_4_t msg_carrier;
	IEEE802_15_4_Header_t *header = msg_carrier.GetHeader();

	header->length = sizeof(Payload_t) + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->src = CPU_Radio_GetAddress(this->radioName);
	header->dest = 3505;

	Payload_t* data_msg = (Payload_t*)msg_carrier.GetPayload();
	msg.MSGID = 1;
	for(int i = 1; i <= PAYLOAD_SIZE; i++){
		msg.data[i-1] = i;
	}
	*data_msg = msg;

	return msg_carrier;*/
}

void RadioTestReceive::SendPacket()
{
	/*Message_15_4_t txMsg;
	Message_15_4_t* txMsgPtr = &txMsg;
	txMsgPtr = (Message_15_4_t *) CPU_Radio_Send_TimeStamped(this->radioName, &msg_carrier, (msg_carrier.GetHeader())->GetLength(), HAL_Time_CurrentTicks());*/
}

BOOL RadioTestReceive::StartTest()
{
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Start(TEST_0A_TIMER);
	ASSERT(rm == TimerSupported);

	return TRUE;
}

BOOL RadioTestReceive::Initialize()
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Test_0A_Timer_Pin_30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin_30, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Test_0A_Timer_Pin_31, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) Test_0A_Timer_Pin_31, FALSE);

	initialPacketReceived = false;
	receiveCount = 0;
	//myAddress = 6846;
	radioName = RF231RADIO;
	DeviceStatus status;
	Radio_Event_Handler.SetRadioInterruptHandler(RadioTest_InterruptHandler);
	Radio_Event_Handler.SetSendAckHandler(RadioTest_SendAckHandler);
	Radio_Event_Handler.SetReceiveHandler(RadioTest_ReceiveHandler);
	status = grf231Radio.Initialize(&Radio_Event_Handler, this->radioName, 1);
	myAddress = grf231Radio.GetAddress();
	grf231Radio.TurnOnRx();

	hal_printf("My address is: %d; I am in receive mode\n", myAddress);

	//Weird! VirtTimer_Initialize is needed to run this test in master branch, but not needed while running in OMAC_Master_Final branch
	//VirtTimer_Initialize();
	/*VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(TEST_0A_TIMER, 0, 500, TRUE, FALSE, Test_0A_Timer_Handler);
	ASSERT(rm == TimerSupported);*/

	return TRUE;
}

void RadioTest_Initialize()
{
	g_RadioTestReceive.Initialize();
	g_RadioTestReceive.StartTest();
}


