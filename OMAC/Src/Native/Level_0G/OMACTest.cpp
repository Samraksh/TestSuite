/*
 * TimesyncTest.cpp
 */
#include <tinyhal.h>
//#include <Samraksh/HALTimer.h>
#include <Samraksh/VirtualTimer.h>
#include "OMACTest.h"


//extern HALTimerManager gHalTimerManagerObject;
//extern VirtualTimer gVirtualTimerObject;
//extern VirtualTimerMapper gVirtualTimerMapperObject;
extern OMACTest gOMACTest;
extern OMACScheduler g_omac_scheduler;
extern OMACType g_OMAC;
extern UINT16 MF_NODE_ID;
//extern Buffer_15_4_t m_receive_buffer;

#define NEIGHBORCLOCKMONITORPIN 31 //2
#define LOCALCLOCKMONITORPIN 22 //25

#define LocalClockMonitor_TIMER1 32
#define NeighborClockMonitor_TIMER1 33

#define USEONESHOTTIMER FALSE

//NEIGHBORCLOCKMONITORPERIOD in ticks
#define NEIGHBORCLOCKMONITORPERIOD 4000000
#define INITIALDELAY 100000

void OMACTest_ReceiveHandler (void* msg, UINT16 NumOfPendingPackets){
}

void OMACTest_SendAckHandler (void* msg, UINT16 size, NetOpStatus status){
}

void CMaxTSLocalClockMonitorTimerHandler(void * arg) {
	//Toggle Pin State for monitoring with Logic Analyzer
	if(gOMACTest.LocalClkPINState){
		CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, false);
		gOMACTest.LocalClkPINState = false;
	}
	else {
		CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, true);
		gOMACTest.LocalClkPINState = true;
	}
	BOOL rv = gOMACTest.ScheduleNextLocalCLK();
}

void CMaxTSNeighborClockMonitorTimerHandler(void * arg) {
	//Toggle Pin State for monitoring with Logic Analyzer
	//gOMACTest.IsNeighborCLKScheduled = false;
	UINT16 Nbr2beFollowed = g_OMAC.Neighbor2beFollowed;
	if(g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) > 2 ) {//if ( g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) >= 5 ))
		if(gOMACTest.NeighborClkPINState){
			CPU_GPIO_SetPinState((GPIO_PIN) NEIGHBORCLOCKMONITORPIN, false);
			gOMACTest.NeighborClkPINState = false;
		}
		else {
			CPU_GPIO_SetPinState((GPIO_PIN) NEIGHBORCLOCKMONITORPIN, true);
			gOMACTest.NeighborClkPINState = true;
		}
	}
	BOOL rv = gOMACTest.ScheduleNextNeighborCLK();
	//VirtualTimerReturnMessage rm;
	//rm = VirtTimer_Start(NeighborClockMonitor_TIMER);
}


// TIMESYNCTEST

BOOL OMACTest::Initialize(){

	CPU_GPIO_EnableOutputPin((GPIO_PIN) NEIGHBORCLOCKMONITORPIN, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) LOCALCLOCKMONITORPIN, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) NEIGHBORCLOCKMONITORPIN, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) LOCALCLOCKMONITORPIN, FALSE);
	LocalClkPINState = true;
	NeighborClkPINState = true;

	LocalClockMonitorFrameNum = 0;
	NeighborClockMonitorFrameNum = 0;

	MyAppID = 3; //pick a number less than MAX_APPS currently 4.
	Config.Network = 138;
	Config.NeighborLivenessDelay = 20000;
	myEventHandler.SetReceiveHandler(OMACTest_ReceiveHandler);
	myEventHandler.SetSendAckHandler(OMACTest_SendAckHandler);
	VirtTimer_Initialize();

	MacId = OMAC;
	Mac_Initialize(&myEventHandler,MacId, MyAppID, Config.RadioID, (void*) &Config);

	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(LocalClockMonitor_TIMER1, 0, NEIGHBORCLOCKMONITORPERIOD, USEONESHOTTIMER, FALSE, CMaxTSLocalClockMonitorTimerHandler);
	ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_SetTimer(NeighborClockMonitor_TIMER1, 0, NEIGHBORCLOCKMONITORPERIOD, USEONESHOTTIMER, FALSE, CMaxTSNeighborClockMonitorTimerHandler);
	assert(rm == TimerSupported);


	return TRUE;
}

BOOL OMACTest::StartTest(){
	VirtualTimerReturnMessage rm;

	UINT64 y = HAL_Time_CurrentTicks();
	UINT64 LocalClockMonitorFrameNum = y /( (UINT64) NEIGHBORCLOCKMONITORPERIOD );

	ScheduleNextLocalCLK();
	ScheduleNextNeighborCLK();

	rm = VirtTimer_Start(LocalClockMonitor_TIMER1);
	rm = VirtTimer_Start(NeighborClockMonitor_TIMER1);

	return TRUE;
}

BOOL OMACTest::ScheduleNextNeighborCLK(){
	UINT16 Nbr2beFollowed = g_OMAC.Neighbor2beFollowed;
	VirtualTimerReturnMessage rm;

	if (g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) > 2 ) {//if ( g_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) >= 5 ){
		UINT64 NextEventTime = 0, neighborTime = 0, y = 0;
		UINT64 LocalFireUpTime = 0;
		UINT64 TicksTillNextEvent;


		//y = HAL_Time_CurrentTicks();
		while (LocalFireUpTime <= y || TicksTillNextEvent <= 400000){
			y = HAL_Time_CurrentTicks();
			neighborTime = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(Nbr2beFollowed, y);
			//neighborTime = y;
			while(NextEventTime <= neighborTime){
				NextEventTime = ( ++NeighborClockMonitorFrameNum ) * ((UINT64)NEIGHBORCLOCKMONITORPERIOD);
			}
			LocalFireUpTime = g_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(Nbr2beFollowed, NextEventTime);
			//LocalFireUpTime = NextEventTime;
			TicksTillNextEvent = LocalFireUpTime - y;
		}
		UINT32 MicSTillNextEvent = (UINT32) (HAL_Time_TicksToTime(TicksTillNextEvent));
		UINT32 ProcessingLatency = (UINT32) (HAL_Time_TicksToTime( HAL_Time_CurrentTicks() - y));
		rm = VirtTimer_Change(NeighborClockMonitor_TIMER1, 0, MicSTillNextEvent + ProcessingLatency, USEONESHOTTIMER);
		return TRUE;
	}
	else {
		return FALSE;
	}


}

BOOL OMACTest::ScheduleNextLocalCLK(){
		VirtualTimerReturnMessage rm;
		UINT64 y = HAL_Time_CurrentTicks();
		UINT64 NextEventTime = 0;
		while(NextEventTime <= y){
			NextEventTime = (++LocalClockMonitorFrameNum) * ((UINT64)NEIGHBORCLOCKMONITORPERIOD);
		}
		UINT64 TicksTillNextEvent = NextEventTime - y;
		UINT32 MicSTillNextEvent = (UINT32) (HAL_Time_TicksToTime(TicksTillNextEvent)) ;
		rm = VirtTimer_Change(LocalClockMonitor_TIMER1, 0, MicSTillNextEvent, USEONESHOTTIMER);
		ASSERT_SP(rm == TimerSupported);
		//rm = VirtTimer_Start(LocalClockMonitor_TIMER1);
		//ASSERTASSERT_SP == TimerSupported);
		return TRUE;

}

void OMACTest_Initialize(){
	BOOL ret;
	ret = gOMACTest.Initialize();
	ret = gOMACTest.StartTest();
}


