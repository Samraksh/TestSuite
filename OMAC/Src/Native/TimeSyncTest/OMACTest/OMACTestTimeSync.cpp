/* TimeSync  test
  This test is intended to facilitate measuring the time sync accuracy between two nodes.
  The accuracy is tested by monitoring and comparing the instances of GPIO toggling in between two nodes. A logic analyzer can be used to record these instances. 
  
  The test uses two GPIO pins, LOCALCLOCKMONITORPIN, NEIGHBORCLOCKMONITORPIN.
  LOCALCLOCKMONITORPIN is toggled periodically with NEIGHBORCLOCKMONITORPERIOD starting from clock = 0. In other words it is toggled whenever the clock value becomes divisible by NEIGHBORCLOCKMONITORPERIOD. Note that this is more stringent then just periodic and takes into account the constant offset. 
  
  NEIGHBORCLOCKMONITORPIN is toggled when the estimate of the neigbor's clock becomes divisible by NEIGHBORCLOCKMONITORPERIOD.
  
  The first discovered neighbor on each node is targeted. 
  
  Special consideration is added for Austere nodes. 
 */

#include "OMACTestTimeSync.h"

#define AUSTERE_NODE_ID_1 8334
#define AUSTERE_NODE_ID_2 14728

#define OMACTEST_PRINT_TIME_MISMATCH 0
#define OMACTEST_PRINT_RX_PACKET_INFO 1


extern OMACType g_OMAC;
extern UINT16 MF_NODE_ID;

#define MINEVENTTIM 1000

#define USEONESHOTTIMER FALSE

//NEIGHBORCLOCKMONITORPERIOD in ticks
//#define NEIGHBORCLOCKMONITORPERIOD 200000 800000
#define NEIGHBORCLOCKMONITORPERIOD_MICRO (100000)
#define NEIGHBORCLOCKMONITORPERIOD (g_OMAC.m_Clock.ConvertMicroSecstoTicks(NEIGHBORCLOCKMONITORPERIOD_MICRO))
#define INITIALDELAY 100000



void CMaxTSLocalClockMonitorTimerHandler(void * arg) {
	dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->LocalClockMonitorTimerHandler(arg);
}
void CMaxTSNeighborClockMonitorTimerHandler(void * arg) {
	dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->NeighborClockMonitorTimerHandler(arg);
}


void OMACTestTimeSync::LocalClockMonitorTimerHandler(void * arg) {
	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Stop(LocalClockMonitor_TIMER1);
	ASSERT_SP(rm == TimerSupported);


	//Toggle Pin State for monitoring with Logic Analyzer
	if(dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->LocalClkPINState){
		CPU_GPIO_SetPinState(m_LOCALCLOCKMONITORPIN, false);
		dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->LocalClkPINState = false;
	}
	else {
		CPU_GPIO_SetPinState(m_LOCALCLOCKMONITORPIN, true);
		dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->LocalClkPINState = true;
	}
	BOOL rv = dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->ScheduleNextLocalCLK();


	rm = VirtTimer_Start(LocalClockMonitor_TIMER1);
	ASSERT_SP(rm == TimerSupported);

}

void OMACTestTimeSync::NeighborClockMonitorTimerHandler(void * arg) {


	VirtualTimerReturnMessage rm;
	rm = VirtTimer_Stop(NeighborClockMonitor_TIMER1);
	ASSERT_SP(rm == TimerSupported);

	//Toggle Pin State for monitoring with Logic Analyzer
	//UINT16 Nbr2beFollowed = g_OMAC.Neighbor2beFollowed;
	UINT16 Nbr2beFollowed;
	if(g_NeighborTable.Neighbor[0].IsAvailableForUpperLayers == true){
		Nbr2beFollowed = g_NeighborTable.Neighbor[0].MACAddress;
	}
	else{
		Nbr2beFollowed = 0;
	}
	if(g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) > 2 ) {//if ( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) >= 5 ))
		if(dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->NeighborClkPINState){
			CPU_GPIO_SetPinState(m_NEIGHBORCLOCKMONITORPIN, false);
			dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->NeighborClkPINState = false;
		}
		else {
			CPU_GPIO_SetPinState(m_NEIGHBORCLOCKMONITORPIN, true);
			dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->NeighborClkPINState = true;
		}
	}
	BOOL rv = dynamic_cast<OMACTestTimeSync*>(gOMACTestBaseptr)->ScheduleNextNeighborCLK();



	rm = VirtTimer_Start(NeighborClockMonitor_TIMER1);
	ASSERT_SP(rm == TimerSupported);
}


// TIMESYNCTEST

BOOL OMACTestTimeSync::Initialize(MACReceiveFuncPtrType rx_fptr
		, SendAckFuncPtrType sa_fptr
		, NeighborChangeFuncPtrType nc_fptr
		){
	this->OMACTestBase::Initialize(rx_fptr,sa_fptr, nc_fptr);

#ifdef AUSTERE_RADIO_GPIO_PIN
	if(AUSTERE_RADIO_GPIO_PIN == DISABLED_PIN ) {
		hal_printf("OMACTestBase: Periodic messaging test is running with AUSTERE_RADIO_GPIO_PIN disabled!!");
	}
	else if(g_OMAC.GetMyID() == AUSTERE_NODE_ID_1){
		this->m_LOCALCLOCKMONITORPIN = AUSTERE_RADIO_GPIO_PIN;
		this->m_NEIGHBORCLOCKMONITORPIN = DISABLED_PIN;
		hal_printf("OMACTestBase: AUSTERE_NODE_ID_1");
	}
	else if(g_OMAC.GetMyID() == AUSTERE_NODE_ID_2){
		this->m_LOCALCLOCKMONITORPIN = DISABLED_PIN;
		this->m_NEIGHBORCLOCKMONITORPIN = AUSTERE_RADIO_GPIO_PIN;
		hal_printf("OMACTestBase: AUSTERE_NODE_ID_2");
	}
	else{
		hal_printf("OMACTestBase periodic messaging test is running with UNKNOWN IDS!!");
	}
#else
	this->m_LOCALCLOCKMONITORPIN = LOCALCLOCKMONITORPIN ;
	this->m_NEIGHBORCLOCKMONITORPIN = NEIGHBORCLOCKMONITORPIN;
#endif

	sequence_number = 0;
	rx_packet_count = 0;

	CPU_GPIO_EnableOutputPin(m_NEIGHBORCLOCKMONITORPIN, TRUE);
	CPU_GPIO_EnableOutputPin(m_LOCALCLOCKMONITORPIN, TRUE);
	CPU_GPIO_SetPinState(m_NEIGHBORCLOCKMONITORPIN, FALSE);
	CPU_GPIO_SetPinState(m_LOCALCLOCKMONITORPIN, FALSE);

	LocalClkPINState = true;
	NeighborClkPINState = true;

	LocalClockMonitorFrameNum = 0;
	NeighborClockMonitorFrameNum = 0;

	NeighborFound = false;


	
	VirtTimer_Initialize();


	VirtualTimerReturnMessage rm;
	rm = VirtTimer_SetTimer(LocalClockMonitor_TIMER1, 0, NEIGHBORCLOCKMONITORPERIOD_MICRO, USEONESHOTTIMER, FALSE, CMaxTSLocalClockMonitorTimerHandler, OMACClockSpecifier);
	ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_SetTimer(NeighborClockMonitor_TIMER1, 0, NEIGHBORCLOCKMONITORPERIOD_MICRO, USEONESHOTTIMER, FALSE, CMaxTSNeighborClockMonitorTimerHandler, OMACClockSpecifier);
	ASSERT_SP(rm == TimerSupported);


	return TRUE;
}

BOOL OMACTestTimeSync::StartTest(){
	VirtualTimerReturnMessage rm;

	UINT64 y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
	UINT64 LocalClockMonitorFrameNum = y /( (UINT64) NEIGHBORCLOCKMONITORPERIOD );

	ScheduleNextLocalCLK();
	ScheduleNextNeighborCLK();

	rm = VirtTimer_Start(LocalClockMonitor_TIMER1);
	ASSERT_SP(rm == TimerSupported);
	rm = VirtTimer_Start(NeighborClockMonitor_TIMER1);
	ASSERT_SP(rm == TimerSupported);

	return TRUE;
}

BOOL OMACTestTimeSync::ScheduleNextNeighborCLK(){
	UINT16 Nbr2beFollowed;
	if(g_NeighborTable.Neighbor[0].IsAvailableForUpperLayers == true){
		Nbr2beFollowed = g_NeighborTable.Neighbor[0].MACAddress; //g_NeighborTable.Neighbor[0].MACAddress;
		if(!NeighborFound){
			NeighborFound = true;
			hal_printf("\n NEIGHBOR FOUND!! \n");
		}
	}
	else{
		Nbr2beFollowed = 0;
		if(NeighborFound){
			NeighborFound = false;
			hal_printf("\n NEIGHBOR LOST!! \n");
		}
	}
	VirtualTimerReturnMessage rm;

	if (g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) > 2 ) {//if ( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(Nbr2beFollowed) >= 5 ){
		UINT64 NextEventTime = 0, neighborTime = 0, y = 0;
		UINT64 LocalFireUpTime = 0;
		UINT64 TicksTillNextEvent;


		//y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
		while (LocalFireUpTime <= y || TicksTillNextEvent <= MINEVENTTIM){
			y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
			neighborTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Local2NeighborTime(Nbr2beFollowed, y);
			//neighborTime = y;
			NeighborClockMonitorFrameNum =  neighborTime / ((UINT64)NEIGHBORCLOCKMONITORPERIOD);
			while(NextEventTime <= neighborTime){
				NeighborClockMonitorFrameNum = NeighborClockMonitorFrameNum + 1;
				NextEventTime = ( NeighborClockMonitorFrameNum ) * ((UINT64)NEIGHBORCLOCKMONITORPERIOD);
			}
			LocalFireUpTime = g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.Neighbor2LocalTime(Nbr2beFollowed, NextEventTime);
			//LocalFireUpTime = NextEventTime;
			TicksTillNextEvent = LocalFireUpTime - y;
		}
		UINT32 MicSTillNextEvent = (UINT32) (g_OMAC.m_Clock.ConvertTickstoMicroSecs(TicksTillNextEvent));
		UINT32 ProcessingLatency = (UINT32) (g_OMAC.m_Clock.ConvertTickstoMicroSecs( g_OMAC.m_Clock.GetCurrentTimeinTicks() - y));
		rm = VirtTimer_Change(NeighborClockMonitor_TIMER1, 0, MicSTillNextEvent + ProcessingLatency, USEONESHOTTIMER, OMACClockSpecifier);
		ASSERT_SP(rm == TimerSupported);

		++(sequence_number);
		//hal_printf("\n sequence_number = %Lu \n", sequence_number);
//		if( (sent_packet_count % 20 < 10 && sequence_number % 100 == 0)
//		||  (sent_packet_count % 20 >= 10 && sequence_number % 1000 == 0)
//				) {
//				if(g_OMAC.Send(g_NeighborTable.Neighbor[sent_packet_count%(g_NeighborTable.NumberOfNeighbors())].MACAddress, 128, &sequence_number, sizeof(UINT64))){
//					hal_printf("\n PACKET ACCEPTED!! \n");
//					++sent_packet_count;
//				}
//				else{
//					hal_printf("\n SEND FAILED!! \n");
//				}
//		}


		return TRUE;
	}
	else {
		return FALSE;
	}


}

BOOL OMACTestTimeSync::ScheduleNextLocalCLK(){
		VirtualTimerReturnMessage rm;
		UINT64 y = g_OMAC.m_Clock.GetCurrentTimeinTicks();
		UINT64 NextEventTime = 0;
		UINT64 TicksTillNextEvent = 0;
		while(NextEventTime <= y || TicksTillNextEvent <= MINEVENTTIM ){
			LocalClockMonitorFrameNum = LocalClockMonitorFrameNum + 1;
			NextEventTime = LocalClockMonitorFrameNum * (g_OMAC.m_Clock.ConvertMicroSecstoTicks(NEIGHBORCLOCKMONITORPERIOD_MICRO));
			TicksTillNextEvent = NextEventTime - y;
		}

		UINT32 MicSTillNextEvent = (UINT32) (g_OMAC.m_Clock.ConvertTickstoMicroSecs(TicksTillNextEvent)) ;
		rm = VirtTimer_Change(LocalClockMonitor_TIMER1, 0, MicSTillNextEvent, USEONESHOTTIMER, OMACClockSpecifier);
		ASSERT_SP(rm == TimerSupported);


		return TRUE;
}


