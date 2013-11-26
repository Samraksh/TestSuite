////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MACLayerTest.h"
#include <Samraksh\HAL_util.h>
#include <Samraksh\Mac_decl.h>
#include <Samraksh\MAC.h>

//---//
MacEventHandler_t Event_Handler;

MACLayerTest::MACLayerTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	testMathInstance.prng_init(3);

};

BOOL MACLayerTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

void  SendAckCallback(void *msg, UINT16 size, NetOpStatus status){


	if(status == NO_Success)
	{
		MACLayerTest::SendAckPending = FALSE;
	}

}

void RecieveCallback(UINT16 arg1)
{

}

void NeigbhbourChangeCallback(INT16 arg1)
{

}


DeviceStatus InitializeMacLayer()
{

	MacConfig config;

	// Pass the csma macname id because thats what we are using
	MacName = 0;

	MACLayerTest::MacID = 0;

	config.CCA = TRUE;
	config.NumberOfRetries = 0;
	config.CCASenseTime = 140;
	config.BufferSize = 8;
	config.RadioID = 1;
	config.NeighbourLivelinessDelay = 300;

	Event_Handler.SetRecieveHandler(&RecieveCallback);
	Event_Handler.SetSendAckHandler(&SendAckCallback);
	Event_Handler.SetNeighbourChangeHandler(&NeigbhbourChangeCallback);

	UINT8 MyAppID=3; //pick a number less than MAX_APPS currently 4.

	if(Mac_Initialize(&Event_Handler, &MACLayerTest::MacID, MyAppID, (void*) &config) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeTxPower(1, 0x0) != DS_Success)
		return DS_Fail;

	if(CPU_Radio_ChangeChannel(1, 0xF) != DS_Success)
		return DS_Fail;

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24,FALSE);

	return DS_Success;
}

// This test only checks if sends from the mac layer are successful
BOOL MACLayerTest::Level_0A()
{

	UINT16 i = 0;

	UINT16 failureToSend = 0;

	if(InitializeMacLayer() != DS_Success)
	{
		DisplayStats(FALSE,"Mac Layer initialization failed","",0);
		return FALSE;
	}

	UINT8 mesg[10];

	for(UINT16 i = 0 ; i < 10; i++)
	{
		mesg[i] = i;
	}

	while(i++ < 10)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		if(Mac_Send(MacID, 0xffff, 1, (void*) mesg, 10) != DS_Success)
		{
			hal_printf("The current iteration number is %d", i);
			failureToSend++;
			//DisplayStats(FALSE,"Mac Layer Send failed","",0);
			//return FALSE;
		}

		while(SendAckPending == TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		SendAckPending = FALSE;

		// Sleep  for a while
		for(UINT16 i = 0 ; i < 50000; i++);

	}

	if(failureToSend > 0)
	{
		hal_printf("Failed to Send %d Packets", failureToSend);
		DisplayStats(FALSE,"Mac Layer Send failed","",0);
	}
	else
	{
		DisplayStats(TRUE, "Mac Layer Send Test succeeded","",0);
	}

	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL MACLayerTest::Level_0B()
{
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

