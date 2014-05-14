////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "csmaMACTest.h"
#include <..\DeviceCode\pal\COM\Wireless\MAC\CSMAMAC\csmaMAC.h>
#include <radio\RF231\RF231.h>
//#include <..\pal\COM\Radio\MAC\OMAC\OMAC.h>

//extern OMAC g_OMAC;
//---//
extern csmaMAC gcsmaMacObject;
MacEventHandler_t Event_Handler;


void  SendAckCallback(void *msg, UINT16 size, NetOpStatus status)
{
	/*if(status == NO_Success)
	{
		MACLayerTest::SendAckPending = FALSE;
	}*/
}


void RecieveCallback(UINT16 arg1)
{
}

void NeigbhbourChangeCallback(INT16 arg1)
{
}

csmaMACTest::csmaMACTest( int seedValue, int numberOfEvents )
{
	MacConfig config;
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

	gcsmaMacObject.Initialize(&Event_Handler, CSMAMAC, MyAppID, config.RadioID, &config);
	
};

BOOL csmaMACTest::Level_0A()
{
	UINT8 sendmsg[] = {0,1,0,2,0,3,0,4,0,5,0,6};
	while(true)
	{
		hal_printf("Sending msg \n");
		gcsmaMacObject.Send(0xffff, 0, sendmsg, 12);
		for(int i = 0; i < 150000; i++);
	}
	return TRUE;

}

BOOL csmaMACTest::Level_0B()
{

}




BOOL csmaMACTest::Execute( int testLevel )
{
	if(testLevel == LEVEL_0A)
		Level_0A();
} //Execute

