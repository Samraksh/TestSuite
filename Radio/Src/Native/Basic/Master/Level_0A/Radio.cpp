////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Radio.h"

//---//

RadioTest radioObject;

extern "C"
{

void* myReceiveHandler (void *msg, UINT16 Size){

	UINT8 * data = (UINT8 *) msg;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);

	if(radioObject.currentpackedId == data[0])
		radioObject.successfulTestCount++;

	RadioTest::RadioReceivedPending = FALSE;

	Message_15_4_t** temp = &recv_mesg_carrier_ptr;
	recv_mesg_carrier_ptr = (Message_15_4_t *) msg;
	return (void *) *temp;

}
}

void RadioTest::SendAckHandler(void *msg, UINT16 Size, NetOpStatus state){

	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

	RadioAckPending = FALSE;

}



BOOL RadioTest::Initialize(int seedValue, int numberOfEvents)
{

	hal_printf("Check Point 1.a\n");
	//CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	hal_printf("Check Point 1.b\n");
	RadioAckPending = FALSE;
	radioID = 1;
	numberOfRadios = 1;
	mac_id = 1;
	DeviceStatus result;

	radioEventHandler.SetRecieveHandler(&myReceiveHandler);
	radioEventHandler.SetSendAckHandler((void (*)(void*, UINT16, NetOpStatus)) &RadioTest::SendAckHandler);

	hal_printf("Check Point 1.c\n");
	result = CPU_Radio_Initialize(&radioEventHandler , radioID, numberOfRadios, mac_id );

	hal_printf("Check Point 1.d\n");
	if(result != DS_Success)
	{
		DisplayStats(FALSE,"Radio Initialization failed", NULL, NULL);
		return FALSE;
	}

	this->numberOfEvents = numberOfEvents;

	msg_carrier_ptr = & msg_carrier;

	hal_printf("Check Point 1.e\n");
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24 , FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25 , FALSE);

	hal_printf("Check Point 1.f\n");
	successfulTestCount = 0;
}

BOOL RadioTest::Level_0A()
{
	DeviceStatus result;
	BOOL SendResult;
	UINT32 i = 0;
	UINT8 regValue;

	result = CPU_Radio_TurnOn(radioID);

	if(result == DS_Success)
	{
		/*regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
		if(regValue != RF230_RX_ON)
		{
			DisplayStats(FALSE, "Read register failed after turn on",NULL, NULL);
			return FALSE;
		}*/
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

BOOL RadioTest::Level_0B()
{
	DeviceStatus result;
		BOOL SendResult;
		UINT32 i = 0;
		UINT8 regValue;
		INIT_STATE_CHECK();

		result = CPU_Radio_TurnOn(radioID);

		if(result == DS_Success)
		{
			regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
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

				currentpackedId = i;

				hal_printf("Transmitting Packet %d", i);
				// Try sending a packet after turning it on
				SendResult = SendPacketSync(MAC_BROADCAST_ADDRESS, MFM_DATA, (void *) &msg.data, sizeof(Payload_t));

				//while(RadioAckPending == TRUE);
				DID_STATE_CHANGE(RadioAckPending,"Timed out waiting for send done ack");

				DID_STATE_CHANGE(RadioReceivedPending,"Timed out waiting for receive ack from slave");

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

BOOL RadioTest::SendPacketSync(UINT16 dest, UINT8 dataType, void* msg, int Size)
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

BOOL RadioTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, char* accuracy)
{
	hal_printf("result = %s\n", (result) ? "PASS":"FAIL");
	hal_printf("accuracy = %s\n", accuracy);
	hal_printf("resultParameter1 = %s\n", resultParameter1);
	hal_printf("resultParameter2 = %s\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	hal_printf("\r\nDummy Write\r\n");
	hal_printf("\r\nDummy Write\r\n");
	hal_printf("\r\nDummy Write\r\n");
	hal_printf("\r\nDummy Write\r\n");

	USART_Flush(COM1);

	return TRUE;
}

BOOL RadioTest::SleepTest_Level0()
{

	DeviceStatus result;
	UINT8 regValue;

	UINT32 i = 0;

	while(i++ < this->numberOfEvents)
	{

		result = CPU_Radio_TurnOn(radioID);

		if(result == DS_Success)
		{
			regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
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

		result = CPU_Radio_Sleep(radioID, 0);

		if(result == DS_Success)
		{
			regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
			if(regValue != 0)
			{
				DisplayStats(FALSE, "Read register failed after sleep", NULL, NULL);
				return FALSE;
			}
		}
		else
		{
			DisplayStats(FALSE, "Radio Sleep Command failed", NULL, NULL);
			return FALSE;
		}


	}

	DisplayStats(TRUE,"Sleep Level 0 Success", "Radio successfully turns on and goes back to sleep when commanded", NULL);

	// Reset test counter in preparation for next level of testing
	i  = 0;

}

BOOL RadioTest::SleepTest_Level1B()
{
	DeviceStatus result;
	UINT8 regValue;
	BOOL SendResult;

	UINT32 i = 0;

	while(i++ < this->numberOfEvents)
	{
			result = CPU_Radio_TurnOn(radioID);

			if(result == DS_Success)
			{
				regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

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

				result = CPU_Radio_Sleep(radioID,0);

				if(grf231Radio.sleep_pending == TRUE)
				{
					// wait for radio ack pending to be true
					// this means the radio should have seen that a sleep was pending and must have acted on it
					while(RadioAckPending == TRUE);


					regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

					if(regValue != 0)
					{
						DisplayStats(FALSE, "Read register failed after sleep", NULL, NULL);
						return FALSE;
					}
				}

				if(result != DS_Success)
				{
					DisplayStats(FALSE, "Radio Sleep Command failed", NULL, NULL);
					return FALSE;
				}
			}
	}

	DisplayStats(TRUE,"Sleep Level 1B Success", "Radio successfully turns on, sends a packet and goes back to sleep when commanded", NULL);

}

// This test is designed to test if the radio can move from sleep to send to sleep back again without explicity using the
// TurnOn
BOOL RadioTest::SleepTest_Level1C()
{
	DeviceStatus result;
	UINT8 regValue;
	BOOL SendResult;

	UINT32 i = 0;

	//while(i++ < this->numberOfEvents)
	while(TRUE)
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

				// wait for radio ack pending to be true
				// this means the radio should have seen that a sleep was pending and must have acted on it
			while(RadioAckPending == TRUE);

			regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

			if(regValue != 0)
			{
				DisplayStats(FALSE, "Read register failed after sleep", NULL, NULL);
				return FALSE;
			}
		}
	}


	DisplayStats(TRUE,"Sleep Level 1C Success", "Radio successfully turns on, sends a packet and goes back to sleep when commanded", NULL);

}

// In level1 we will attempt to add send to the list of states the radio must go through and test if we
// are still able to go to sleep successfully and successfully detect scenarios when the radio can not
// and shound not be sleeping
BOOL RadioTest::SleepTest_Level1A()
{
	DeviceStatus result;
	UINT8 regValue;
	BOOL SendResult;

	m_send_buffer.Initialize();

	UINT32 i = 0;

	while(i++ < this->numberOfEvents)
	{
		result = CPU_Radio_TurnOn(radioID);

		if(result == DS_Success)
		{
			regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
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

			if(SendResult == FALSE)
			{
				DisplayStats(FALSE, "Level 1 Send Failed", NULL, NULL);
				return FALSE;
			}

			// wait for send ack
			while(RadioAckPending == true);
		}

		result = CPU_Radio_Sleep(radioID,0);

		if(result == DS_Success)
		{
			regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
			if(regValue != 0)
			{
				DisplayStats(FALSE, "Read register failed after sleep", NULL, NULL);
				return FALSE;
			}
		}
		else
		{
			DisplayStats(FALSE, "Radio Sleep Command failed", NULL, NULL);
			return FALSE;
		}
	}

	DisplayStats(TRUE,"Sleep Level 1 Success", "Radio successfully turns on, sends a packet and goes back to sleep when commanded", NULL);
}

BOOL RadioTest::Execute( int testLevel )
{
	BOOL result;

	if(testLevel == LEVEL_0A)
		result = Level_0A();

	if(testLevel == LEVEL_0B)
		result = Level_0B();

	if(testLevel == SLEEPTEST)
	{
		// result = SleepTest_Level0();
		// result &= SleepTest_Level1A();
		// result &= SleepTest_Level1B();
		 result = SleepTest_Level1C();
	}

	return result;



} //Execute

