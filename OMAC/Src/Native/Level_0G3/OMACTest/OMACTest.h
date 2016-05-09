/*
 * OMACTest.h
 */

#ifndef OMACTest_H_
#define OMACTest_H_



#include <tinyhal.h>
#include <Samraksh/MAC_decl.h>
#include <Samraksh/MAC/OMAC/OMAC.h>
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Message.h>


class OMACTest{
public:
	OMACTest();
	GlobalTime m_globalTime;
	UINT8 MyAppID;
	MACEventHandler myEventHandler;
	MACConfig Config;
	UINT8 MacId;
	BOOL LocalClkPINState;
	BOOL NeighborClkPINState;

	UINT64 LocalClockMonitorFrameNum;
	UINT64 NeighborClockMonitorFrameNum;

	BOOL Initialize();
	BOOL StartTest();

	BOOL ScheduleNextNeighborCLK();
	BOOL ScheduleNextLocalCLK();
};

OMACTest gOMACTest;

void OMACTest_Initialize();


#endif /* OMACTest_H_ */


