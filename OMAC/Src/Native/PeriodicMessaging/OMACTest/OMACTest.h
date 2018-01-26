/*
 * OMACTestPeriodicMessaging.h
 */

#ifndef OMACTestPeriodicMessaging_H_
#define OMACTestPeriodicMessaging_H_



#include "OMACTestBase.h"


class OMACTestTimeSync : public OMACTestBase{
public:
	UINT64 sequence_number;
	UINT64 sent_packet_count;
	UINT64 rx_packet_count;

	GPIO_PIN m_LOCALCLOCKMONITORPIN;
	GPIO_PIN m_NEIGHBORCLOCKMONITORPIN;

	OMACTestPeriodicMessaging();

	GlobalTime m_globalTime;
	UINT8 MyAppID;
	MACEventHandler myEventHandler;
	MACConfig Config;
	UINT8 MacId;
	BOOL LocalClkPINState;
	BOOL NeighborClkPINState;

	UINT64 LocalClockMonitorFrameNum;
	UINT64 NeighborClockMonitorFrameNum;

	UINT64 TargetTimeinTicks;

	bool NeighborFound;

	BOOL Initialize();
	BOOL StartTest();

	BOOL ScheduleNextNeighborCLK();
	BOOL ScheduleNextLocalCLK();
	void SendPacketToNeighbor();
};

OMACTestPeriodicMessaging gOMACTestPeriodicMessaging;

void OMACTestPeriodicMessaging_Initialize();


#endif /* OMACTestPeriodicMessaging_H_ */


