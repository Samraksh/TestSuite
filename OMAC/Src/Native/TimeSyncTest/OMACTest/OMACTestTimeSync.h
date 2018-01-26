/*
 * OMACTestTimeSync.h
 */

#ifndef OMACTestTimeSync_H_
#define OMACTestTimeSync_H_



#include "../../OMACTestBase/OMACTestBase.h"

class OMACTestTimeSync: public OMACTestBase{
public:

	GPIO_PIN m_LOCALCLOCKMONITORPIN;
	GPIO_PIN m_NEIGHBORCLOCKMONITORPIN;


	UINT64 sequence_number;
	UINT64 rx_packet_count;




	BOOL LocalClkPINState;
	BOOL NeighborClkPINState;

	UINT64 LocalClockMonitorFrameNum;
	UINT64 NeighborClockMonitorFrameNum;

	bool NeighborFound;

	OMACTestTimeSync();



	virtual BOOL Initialize(  MACReceiveFuncPtrType rx_fptr = &OMACTestBase_ReceiveHandler
					, SendAckFuncPtrType sa_fptr = &OMACTestBase_SendAckHandler
					, NeighborChangeFuncPtrType nc_fptr = &OMACTestBase_NeighborChangeHandler
					);
	BOOL StartTest();

	void LocalClockMonitorTimerHandler(void * arg);
	void NeighborClockMonitorTimerHandler(void * arg);

	BOOL ScheduleNextNeighborCLK();
	BOOL ScheduleNextLocalCLK();
};

OMACTestTimeSync gOMACTestTimeSync;
OMACTestTimeSync* gOMACTestptr;

void OMACTestTimeSync_Initialize();


#endif /* OMACTestTimeSync_H_ */


