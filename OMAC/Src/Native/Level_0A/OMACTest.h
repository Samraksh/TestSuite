/*
 * OMACTest.h
 */

#ifndef OMACTest_H_
#define OMACTest_H_

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
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/PacketTimeSync_15_4.h>
#include <Samraksh/GlobalTime.h>

class OMACTest{
public:
	OMACTest();
	GlobalTime m_globalTime;
	UINT8 MyAppID;
	MacEventHandler myEventHandler;
	MacConfig Config;
	UINT8 MacId;
	BOOL LocalClkPINState;
	BOOL NeighborClkPINState;
	BOOL IsLocalCLKScheduled;
	BOOL IsNeighborCLKScheduled;


	BOOL Initialize();
	BOOL StartTest();

	BOOL ScheduleNextNeighborCLK();
	BOOL ScheduleNextLocalCLK();
};

OMACTest gOMACTest;

void OMACTest_Initialize();


#endif /* OMACTest_H_ */


