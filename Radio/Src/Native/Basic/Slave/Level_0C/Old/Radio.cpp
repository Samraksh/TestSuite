////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Radio.h"

#include <Samraksh/HALTimer.h>

//---//

extern HALTimerManager gHalTimerManagerObject;
BOOL state = FALSE;

RadioTest gradioObject;

extern "C"
{

void* myReceiveHandler (void *msg, UINT16 Size){

	UINT8 *data = (UINT8 *) msg;

	hal_printf("Received Packet %d", data[0]);

	CPU_GPIO_SetPinState((GPIO_PIN) 4 , TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 4 , FALSE);

	gradioObject.SendPacketSync(MAC_BROADCAST_ADDRESS, MFM_DATA, (void *) data, sizeof(Payload_t));

	Message_15_4_t** temp = &recv_mesg_carrier_ptr;
	recv_mesg_carrier_ptr = (Message_15_4_t *) msg;
	return (void *) *temp;
}

void Timer_1_Handler(void * arg){

	UINT8 regValue;

	if(state == FALSE)
	{
		state = TRUE;
		CPU_GPIO_SetPinState((GPIO_PIN) 8, TRUE);
		if(DS_Success != CPU_Radio_Sleep(1, 0))
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
		}
	}
	else
	{
		regValue = grf231Radio.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

		if(regValue != 0)
		{
			hal_printf("Sleep change failed, radio is still active");
			CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
		}

		CPU_GPIO_SetPinState((GPIO_PIN) 8, FALSE);
		state = FALSE;
		if(DS_Success != CPU_Radio_TurnOn(1))
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
		}
	}
}

}

void RadioTest::SendAckHandler(void *msg, UINT16 Size, NetOpStatus state){
	RadioAckPending = FALSE;

}


BOOL RadioTest::Initialize(int seedValue, int numberOfEvents)
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	RadioAckPending = FALSE;
	radioID = 1;
	numberOfRadios = 1;
	mac_id = 1;
	DeviceStatus result;

	radioEventHandler.SetRecieveHandler(&myReceiveHandler);
	radioEventHandler.SetSendAckHandler((void (*)(void*, UINT16, NetOpStatus)) &RadioTest::SendAckHandler);

	result = CPU_Radio_Initialize(&radioEventHandler , &radioID, numberOfRadios, mac_id );

	if(result != DS_Success)
	{
		DisplayStats(FALSE,"Radio Initialization failed",NULL, NULL);
		return FALSE;
	}

	this->numberOfEvents = numberOfEvents;

	msg_carrier_ptr = & msg_carrier;

	recv_mesg_carrier_ptr = &recv_mesg_carrier;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 22, FALSE);

	gHalTimerManagerObject.Initialize();
}

BOOL RadioTest::Level_0A()
{

	return TRUE;

}

BOOL RadioTest::Level_0B()
{
}

BOOL RadioTest::SleepTest_Reciever_Sleeping()
{
	UINT8 result;

	result = CPU_Radio_TurnOn(radioID);


	result = CPU_Radio_Sleep(radioID, 0);

	if(result != DS_Success)
	{
		hal_printf("The Radio is not sleeping");
		return FALSE;
	}


	while(TRUE)
	{
		for(UINT8 i = 0; i < 10000; i++);
	}

}

BOOL RadioTest::SleepTest_Reciever()
{
	UINT8 result;

	gHalTimerManagerObject.CreateTimer(1, 0, 1000000, FALSE, FALSE, Timer_1_Handler); //1 sec Timer in micro seconds

	result = CPU_Radio_TurnOn(radioID);

	while(TRUE)
	{
		for(UINT8 i = 0; i < 10000; i++);
	}
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
		msg_carrier_ptr = (Message_15_4_t *) CPU_Radio_Send(1, (mymsg), header->length);
	}

	return TRUE;
}

BOOL RadioTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, char* accuracy)
{
	hal_printf("result = %s\n", (result) ? "true":"false");
	hal_printf("accuracy = %s\n", accuracy);
	hal_printf("resultParameter1 = %s\n", resultParameter1);
	hal_printf("resultParameter2 = %s\n", resultParameter2);

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

BOOL RadioTest::SleepTest_Level2()
{
	DeviceStatus result;
	UINT32 i = 0;

	while(i++ < this->numberOfEvents)
	{
		result = CPU_Radio_TurnOn(radioID);
	}
}

BOOL RadioTest::BasicReceiver()
{
	DeviceStatus result;

	result = CPU_Radio_TurnOn(radioID);

	if(result != DS_Success)
	{
		DisplayStats(FALSE,"Radio Turn On failed",NULL, NULL);
		return FALSE;
	}

	while(TRUE)
	{

	}
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

	if(testLevel == BASIC)
	{
		result = BasicReceiver();
	}

	if(testLevel == SLEEPTEST)
	{
#if 0
		 result = SleepTest_Level0();
		 result &= SleepTest_Level1A();
		 result &= SleepTest_Level1B();
		 result &= SleepTest_Level1C();
#endif
		 //result = SleepTest_Reciever();
		 //result = SleepTest_Level2();
		 result = SleepTest_Reciever_Sleeping();
	}

	return result;



} //Execute

