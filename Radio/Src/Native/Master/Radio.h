////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\MAC.h>
#include <Samraksh\Buffer.h>
#include <Samraksh\Radio_decl.h>
#include <..\Targets\Native\STM32F10x\DeviceCode\drivers\radio\RF231\RF231.h>


#define SLEEPTEST 3

typedef Buffer_15_4<8> Buffer_15_4_t;

typedef struct  {
	UINT16 MSGID;
	UINT8 data[5];
}Payload_t;

class RadioTest
{

	RadioEventHandler_t radioEventHandler;
	Buffer_15_4_t m_send_buffer;
	int numberOfEvents;
	Payload_t msg;

	Message_15_4_t msg_carrier;

	Message_15_4_t* msg_carrier_ptr;

	BOOL SendPacketSync(UINT16 dest, UINT8 dataType, void* msg, int Size);

public:
	UINT8 radioID;
	UINT8 numberOfRadios;
	UINT8 mac_id;
	static BOOL RadioAckPending;

//--//
    RadioTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL 	 SleepTest_Level0();

	BOOL 	 SleepTest_Level1A();

	BOOL 	 SleepTest_Level1B();

	BOOL 	 SleepTest_Level1C();

	void 	 SendAckHandler(void *msg, UINT16 Size, NetOpStatus state);

	void* 	 ReceiveHandler (void *msg, UINT16 Size);


	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, char* accuracy);

};

BOOL RadioTest::RadioAckPending = false;


