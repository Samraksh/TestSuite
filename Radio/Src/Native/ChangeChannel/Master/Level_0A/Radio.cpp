////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Radio.h"

#include <Samraksh/HALTimer.h>

//---//

extern HALTimerManager gHalTimerManagerObject;

void* myReceiveHandler (void *msg, UINT16 Size){

	hal_printf("Ack Recieved\n");
	RadioTest::RadioRecvAckPending = FALSE;

	CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);

}

void RadioTest::SendAckHandler(void *msg, UINT16 Size, NetOpStatus state){

	RadioAckPending = FALSE;

}

RadioTest::RadioTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	RadioAckPending = FALSE;
	radioID = 1;
	numberOfRadios = 1;
	mac_id = 1;
	DeviceStatus result;

	radioEventHandler.SetRecieveHandler((void* (*)(void*, UINT16))  &myReceiveHandler);
	radioEventHandler.SetSendAckHandler((void (*)(void*, UINT16, NetOpStatus)) &RadioTest::SendAckHandler);

	result = CPU_Radio_Initialize(&radioEventHandler , &radioID, numberOfRadios, mac_id );

	this->numberOfEvents = numberOfEvents;

	msg_carrier_ptr = & msg_carrier;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 22, FALSE);

	gHalTimerManagerObject.Initialize();


};

BOOL RadioTest::Level_0A()
{

	return TRUE;

}

BOOL RadioTest::Level_0B()
{
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

// Test channel change when the radio is in rx_on state
BOOL RadioTest::ChangeChannel_Level0A()
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

			for(UINT8 c  = 0; c < 15; c++)
			{
				result = CPU_Radio_ChangeChannel(radioID, c);

				regValue = grf231Radio.ReadRegister(RF230_PHY_CC_CCA) & RF230_CHANNEL_MASK;

				if((c + RF230_CHANNEL_OFFSET) != regValue)
				{
					DisplayStats(FALSE, "Change Channel failed", NULL, NULL);
					return FALSE;
				}
			}

		}
	}

	DisplayStats(TRUE, "Change Channel Level 0 Success", "The Radio registers change the channel to the appropriate values", NULL);
	return TRUE;


}

UINT8 RadioTest::ReadRegisterSleep(UINT8 reg)
{
	if(CPU_Radio_TurnOn(radioID) != DS_Success)
		return 0;

	UINT8 regValue = grf231Radio.ReadRegister(reg);

	if(CPU_Radio_Sleep(radioID, 0) != DS_Success)
		return 0;

	return regValue;
}

// Test the change channel when the radio is sleeping
// The radio should wake up process the request and go back to sleep
BOOL RadioTest::ChangeChannel_Level0B()
{
	DeviceStatus result;
	UINT8 regValue;
	BOOL SendResult;
	UINT32 i = 0;

	while(i++ < this->numberOfEvents)
	{
		for(UINT8 c  = 0; c < 15; c++)
		{
			result = CPU_Radio_ChangeChannel(radioID, c);

			if((c + RF230_CHANNEL_OFFSET) != (ReadRegisterSleep(RF230_PHY_CC_CCA) & RF230_CHANNEL_MASK))
			{
				DisplayStats(FALSE, "Channel change failed when radio is sleeping", NULL, NULL);
				return FALSE;
			}
		}
	}

	DisplayStats(TRUE, "Change Channel Level 0b Success", "The Radio registers change the channel to the appropriate values during sleep", NULL);
	return TRUE;
}


BOOL RadioTest::ChangeChannel_Level1A()
{
	DeviceStatus result;
	UINT8 regValue;
	BOOL SendResult;
	UINT32 i = 0;

	m_send_buffer.Initialize();

	result = CPU_Radio_TurnOn(radioID);

	while(i++ < this->numberOfEvents)
	{


		for(UINT8 c  = 0; c < 15; c++)
		{
			result = CPU_Radio_ChangeChannel(radioID, c);

			if((c + RF230_CHANNEL_OFFSET) !=  (grf231Radio.ReadRegister(RF230_PHY_CC_CCA) & RF230_CHANNEL_MASK))
			{
				DisplayStats(FALSE, "Channel change failed when radio is sleeping", NULL, NULL);
				return FALSE;
			}

			hal_printf("Transmitting on channel %d\n", c + 11);

			RadioRecvAckPending = TRUE;

			while(RadioRecvAckPending)
			// Try sending a packet now
			{

				msg.MSGID= i;
				msg.data[0] = 0;
				msg.data[1] = 1;
				msg.data[2] = 2;
				msg.data[3] = 3;
				msg.data[4] = 4;


				CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
				for(UINT32 i = 0; i < 50000; i++);
				CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

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

		}
	}
}

BOOL RadioTest::ChangeTxPower_Level1A()
{
	DeviceStatus result;
		UINT8 regValue;
		BOOL SendResult;
		UINT32 i = 0;

		m_send_buffer.Initialize();

		result = CPU_Radio_TurnOn(radioID);

		//while(i++ < this->numberOfEvents)
		//{


			//for(UINT8 c  = 0; c < 15; c++)
			//{
				UINT8 c = 0x0;

				result = CPU_Radio_ChangeTxPower(radioID, c);

				if((c) !=  (grf231Radio.ReadRegister(RF230_PHY_TX_PWR) & RF230_TX_PWR_MASK))
				{
					DisplayStats(FALSE, "Power change failed", NULL, NULL);
					return FALSE;
				}

				hal_printf("Transmitting with Power %d\n", c);
				hal_printf("Transmitting with Power %d\n", c);

				RadioRecvAckPending = TRUE;

				while(RadioRecvAckPending)
				// Try sending a packet now
				{

					// Send 200 packets
					//if(i == 200)
					//	break;

					msg.MSGID= i;
					msg.data[0] = i & 0xff;
					msg.data[1] = (i & 0xff00) >> 8;
					msg.data[2] = (i & 0xff0000) >> 16;
					msg.data[3] = (i & 0xff000000) >> 24;
					msg.data[4] = 4;

					i++;

					CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
					for(UINT32 j = 0; j < 50000; j++);
					CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

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

			//}
		//}

}

BOOL RadioTest::ChangeTxPower_Level0A()
{
	DeviceStatus result;
		UINT8 regValue;
		BOOL SendResult;
		UINT32 i = 0;

		while(i++ < this->numberOfEvents)
		{
			result = CPU_Radio_TurnOn(radioID);

			if(result != DS_Success)
				continue;

			for(UINT8 power = 0; power < 16; power++)
			{
				result =CPU_Radio_ChangeTxPower(radioID, power);

				if(result == DS_Success)
				{
					regValue = grf231Radio.ReadRegister(RF230_PHY_TX_PWR) & RF230_TX_PWR_MASK;

					if((regValue & RF230_TX_PWR_MASK) != power)
					{
						DisplayStats(FALSE, "Power change failed", NULL, NULL);
						return FALSE;
					}
				}
				else
				{
					DisplayStats(FALSE, "Power change function call returned failure", NULL, NULL);
					return FALSE;
				}
			}

		}

		DisplayStats(TRUE, "Change Power Level 0a Success", "The Radio registers change the power to the appropriate values", NULL);
		return TRUE;
}

BOOL RadioTest::Execute( int testLevel )
{
	BOOL result;

	if(testLevel == SLEEPTEST)
	{
		// result = SleepTest_Level0();
		// result &= SleepTest_Level1A();
		// result &= SleepTest_Level1B();
		 result = SleepTest_Level1C();
	}
	else if(testLevel == CHANGE_CHANNEL_LEVEL_0A)
	{
		result = ChangeChannel_Level0A();
	}
	else if(testLevel == CHANGE_CHANNEL_LEVEL_0B)
	{
		result = ChangeChannel_Level0B();
	}
	else if(testLevel == CHANGE_TXPOWER_LEVEL_0A)
	{
		result = ChangeTxPower_Level0A();
	}
	else if(testLevel == CHANGE_CHANNEL_LEVEL_1A)
	{
		result = ChangeChannel_Level1A();
	}
	else if(testLevel  == CHANGE_TXPOWER_LEVEL_1A)
	{
		result = ChangeTxPower_Level1A();
	}

	return result;



} //Execute

