////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "RadioLRTest.h"
#include <Samraksh\HAL_util.h>
#include <radio\RF231\RF231.h>
//---//
RadioLRTest testObject;

extern "C"
{

void* myReceiveHandler (void *msg, UINT16 Size){

	UINT8 * data = (UINT8 *) msg;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);

	if(testObject.currentpackedId == data[0])
		testObject.successfulTestCount++;

	RadioLRTest::RadioReceivedPending = FALSE;

	Message_15_4_t** temp = &recv_mesg_carrier_ptr;
	recv_mesg_carrier_ptr = (Message_15_4_t *) msg;
	return (void *) *temp;

}

void mySendAckHandler(void *msg, UINT16 size, NetOpStatus state)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

	RadioLRTest::RadioAckPending = FALSE;
}

}


BOOL RadioLRTest::Initialize(int seedValue, int numberOfEvents)
{
	hal_printf("Check Point 1.a\n");
		//CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	hal_printf("Check Point 1.b\n");
	RadioAckPending = FALSE;
	radioID = RF231RADIOLR;
	numberOfRadios = 1;
	mac_id = 1;
	DeviceStatus result;

	radioEventHandler.SetRecieveHandler(&myReceiveHandler);
	radioEventHandler.SetSendAckHandler(&mySendAckHandler);


	result = CPU_Radio_Initialize(&radioEventHandler , radioID, numberOfRadios, mac_id );


	if(result != DS_Success)
	{
		DisplayStats(FALSE,"Radio Initialization failed", NULL, NULL);
		return FALSE;
	}

	this->numberOfEvents = numberOfEvents;

	msg_carrier_ptr = & msg_carrier;


	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24 , FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25 , FALSE);


	successfulTestCount = 0;

	return TRUE;

}

BOOL RadioLRTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

BOOL RadioLRTest::SendPacketSync(UINT16 dest, UINT8 dataType, void* msg, int Size)
{

	IEEE802_15_4_Header_t *header = msg_carrier_ptr->GetHeader();

	if(RadioAckPending == TRUE)
		return FALSE;

	header->length = Size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest =dest;
	header->src = MF_NODE_ID;
	header->network = 138;
	header->mac_id = 1;
	header->type = dataType;

	UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  msg_carrier_ptr->GetPayload();

	for(UINT8 i = 0 ; i < Size; i++)
		payload[i] = lmsg[i];

	Message_15_4_t* mymsg = msg_carrier_ptr;

	if(mymsg != NULL){
		RadioAckPending = TRUE;
		RadioReceivedPending = TRUE;
		msg_carrier_ptr = (Message_15_4_t *) CPU_Radio_Send(1, (mymsg), header->length);
	}

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL RadioLRTest::Level_0A()
{

	DeviceStatus result;
	BOOL SendResult;
	UINT32 i = 0;
	UINT8 regValue;

	result = CPU_Radio_TurnOn(radioID);

	if(result == DS_Success)
	{
		regValue = grf231RadioLR.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
		if(regValue != RF230_RX_ON)
		{
			DisplayStats(FALSE, "Read register failed after turn on",NULL, NULL);
			return FALSE;
		}
	}
	else
	{
		DisplayStats(FALSE,"Turn On Radio failed", NULL, NULL);
		return FALSE;
	}

	while(i++ < this->numberOfEvents)
	{
			// Try sending a packet now
		{
			msg.MSGID= i;
			msg.data[0] = 0;
			msg.data[1] = 1;
			msg.data[2] = 2;
			msg.data[3] = 3;
			msg.data[4] = 4;
				// Try sending a packet after turning it on
			SendResult = SendPacketSync(MAC_BROADCAST_ADDRESS, MFM_DATA, (void *) &msg.data, sizeof(Payload_t));

			while(RadioAckPending == TRUE);

			if(SendResult != TRUE)
			{
				DisplayStats(FALSE, "Send failed", NULL, NULL);
				return FALSE;
			}

		}
	}

	DisplayStats(TRUE,"Send Test Successful", NULL, NULL);
	return TRUE;
}

// This test writes data and then verifies the write has been successful
BOOL RadioLRTest::Level_0B()
{
	return TRUE;

}

BOOL RadioLRTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL RadioLRTest::Level_0C()
{
	return TRUE;
}


BOOL RadioLRTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	case 2:
		result = Level_0C();
		break;
	case 3:
		result = Level_1();
		break;
	}


	return result;



} //Execute

