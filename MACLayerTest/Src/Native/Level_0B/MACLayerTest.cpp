////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MACLayerTest.h"
#include <Samraksh\HAL_util.h>
#include <Samraksh\Mac_decl.h>
#include <Samraksh\MAC.h>
#include <radio\RF231\RF231.h>

//---//
MacEventHandler_t Event_Handler;
UINT16 lastPacketID = 0;

UINT32 failureToExtract = 0;
UINT16 missedResponses = 0;

MACLayerTest::MACLayerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	testMathInstance.prng_init(3);
};

BOOL MACLayerTest::DisplayStats(BOOL result, char* resultParameter1, int resultParameter2, int resultParameter3, int accuracy)
{
	while(true){
		hal_printf("result=%s\n", (result) ? "PASS":"FAIL");
		hal_printf("accuracy=%d\n", accuracy);
		hal_printf("resultParameter1=%s\n", resultParameter1);
		hal_printf("resultParameter2=%d\n", resultParameter2);
		hal_printf("resultParameter3=%d\n", resultParameter3);
		hal_printf("resultParameter4=null\n");
		hal_printf("resultParameter5=null\n");
		HAL_Time_Sleep_MicroSeconds(1000000);
	}
	return TRUE;
}

void  SendAckCallback(void *msg, UINT16 size, NetOpStatus status){
	if(status == NO_Success)
	{
		MACLayerTest::SendAckPending = FALSE;
	}
}

void RecieveCallback(UINT16 numberOfPacketsInBuffer)
{
	UINT8 tempBuffer[128];

	UINT8 *tempBufferPointer = tempBuffer;

	for(UINT16 i = 0 ; i < 128; i++)
	{
		tempBuffer[i] = 0;
	}

	for(UINT16 i = 0; i < numberOfPacketsInBuffer; i++)
	{
		if(Mac_GetNextPacket(&tempBufferPointer) != DS_Success)
		{
			failureToExtract++;
			hal_printf("Failed to Extract this packet");

			return;
		}
	}

	UINT16 Size = tempBuffer[0];
		Size |= (UINT16) (tempBuffer[1] << 8);

	hal_printf("Recieved PACKET : %d of Size : %d\n", (tempBuffer[2] | (tempBuffer[3] << 8)), Size);

	UINT16 packetIDDiff = (tempBuffer[2] | tempBuffer[3] << 8) - lastPacketID;

	if(packetIDDiff != 1)
	{
		missedResponses += packetIDDiff;
	}
	hal_printf("curr: %d last: %d\r\n",packetIDDiff, lastPacketID);
	lastPacketID = (UINT16) (tempBuffer[2] | tempBuffer[3] << 8);

	MACLayerTest::ResponsePending = FALSE;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);

}

void NeigbhbourChangeCallback(INT16 arg1)
{

}


DeviceStatus InitializeMacLayer()
{

	MacConfig config;

	// Pass the csma macname id because thats what we are using
	//MacName = 0;

	MACLayerTest::MacID = CSMAMAC;

	config.CCA = TRUE;
	config.NumberOfRetries = 0;
	config.CCASenseTime = 140;
	config.BufferSize = 8;
	config.RadioID = RF231RADIO;
	config.NeighbourLivelinessDelay = 300;

	Event_Handler.SetRecieveHandler(&RecieveCallback);
	Event_Handler.SetSendAckHandler(&SendAckCallback);
	Event_Handler.SetNeighbourChangeHandler(&NeigbhbourChangeCallback);
	UINT8 MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(Mac_Initialize(&Event_Handler, MACLayerTest::MacID, MyAppID,config.RadioID, (void*) &config) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeTxPower(config.RadioID, 0x0) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeChannel(config.RadioID, 0xF) != DS_Success)
		return DS_Fail;


	CPU_GPIO_EnableOutputPin((GPIO_PIN) 23,FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24,FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25,FALSE);

	return DS_Success;
}

// This test only checks if sends from the mac layer are successful
BOOL MACLayerTest::Level_0A()
{
	return TRUE;
}

// This test writes data and then verifies the write has been successful
BOOL MACLayerTest::Level_0B()
{

	INIT_STATE_CHECK_TEST();

	UINT16 i = 0;
	int attemptsToSend = 0;
	UINT16 failureToSend = 0;

	if(InitializeMacLayer() != DS_Success)
	{
		DisplayStats(FALSE,"Mac Layer initialization failed",0,0,0);
		return FALSE;
	}

	UINT8 mesg[10];

		for(UINT16 i = 0 ; i < 10; i++)
			{
				mesg[i] = (UINT8)i;
			}

		while(TRUE)
		{
			for(UINT16 i = 0 ; i < 10; i++)
			{
				mesg[i] = mesg[i]+1;
			}

			// Use the first byte to send message id of some sort
			mesg[0] = (i & 0xff);
			mesg[1] = ((i >> 8) & 0xff);


			CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
			if(Mac_Send(MacID, 0xffff, 1, (void*) mesg, 10) != DS_Success)
			{
				hal_printf("The current iteration number is %d", i);
				failureToSend++;
			} else {
				hal_printf("Sent %d\r\n",i);
			}

			DID_STATE_CHANGE_TEST(SendAckPending, "Did not recieve send ack from radio");
			CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

			MONITOR_FAILURE(ResponsePending);

			SendAckPending = TRUE;
			ResponsePending = TRUE;

			// Sleep  for a while
			::Events_WaitForEvents( 0, 100 );
			attemptsToSend++;

			if (attemptsToSend > 10){
				hal_printf("All done %d %d %d\r\n",attemptsToSend,failureToSend,missedResponses);
				if(failureToSend > 0)
				{
					hal_printf("Failed to Send %d Packets", failureToSend);
					DisplayStats(FALSE,"Mac Layer Send failed",attemptsToSend,failureToSend,0);
				} else if (missedResponses > 0){
					DisplayStats(TRUE, "Mac Layer Send failed",attemptsToSend,missedResponses,0);
				} else {
					DisplayStats(TRUE, "Mac Layer Send Test succeeded",attemptsToSend,missedResponses,0);
				}
			}
				
		}

	return TRUE;

}

BOOL MACLayerTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL MACLayerTest::Level_0C()
{
	return TRUE;
}


BOOL MACLayerTest::Execute( int testLevel )
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

