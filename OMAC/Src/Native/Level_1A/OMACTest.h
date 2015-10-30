/*
 * OMACTest.h
 */

#ifndef OMACTEST_H_
#define OMACTEST_H_

#define TWO_NODES_TX_RX
#if defined(TWO_NODES_TX_RX)
#define def_Neighbor2beFollowed
#define TXNODEID 3505
#define RXNODEID 6846
#endif


//#define FAN_OUT
//#define FAN_IN

#if defined(FAN_OUT)
#define def_Neighbor2beFollowed2
#define RXNODEID1 3505
#define RXNODEID2 31436
#define TXNODEID 6846
#elif defined(FAN_IN)
#define def_Neighbor2beFollowed2
#define TXNODEID1 3505
#define TXNODEID2 31436
#define RXNODEID 6846
#endif


#include <tinyhal.h>
#include <Samraksh/Mac_decl.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>

const char payloadSize = 5;


typedef struct  {
	UINT32 MSGID;
	char* msgContent;
}Payload_t_ping;


class OMACTest{
public:
	UINT8 MyAppID;
	Payload_t_ping pingPayload;
	MacEventHandler myEventHandler;
	MacConfig Config;
	UINT8 MacId;
	static UINT32 sendPingCount;
	static UINT32 recvCount;
	static UINT32 missedPingID;
	UINT16 RcvCount;

	BOOL Initialize();
	BOOL StartTest();
	void Receive(void* msg, UINT16 size);
	BOOL Send();
	void SendAck(void *msg, UINT16 size, NetOpStatus status);
	void GetStatistics();
};

//extern OMACTest g_OMACTest;

void OMACTest_Initialize();

UINT32 OMACTest::sendPingCount = 0;
UINT32 OMACTest::recvCount = 0;
UINT32 OMACTest::missedPingID = 1;

#endif /* OMACTEST_H_ */



