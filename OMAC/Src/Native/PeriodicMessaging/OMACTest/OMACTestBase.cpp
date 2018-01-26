/*
 * TimesyncTest.cpp
 */

#include "OMACTestBase.h"


//extern HALTimerManager gHalTimerManagerObject;
//extern VirtualTimer gVirtualTimerObject;
//extern VirtualTimerMapper gVirtualTimerMapperObject;
extern OMACTestBase gOMACTestBase;
extern OMACType g_OMAC;
extern UINT16 MF_NODE_ID;
//extern Buffer_15_4_t m_receive_buffer;

#define AUSTERE_NODE_ID_1 8334
#define AUSTERE_NODE_ID_2 14728

#define OMACTestBase_PRINT_TIME_MISMATCH 0
#define OMACTestBase_PRINT_RX_PACKET_INFO 1

#define MINEVENTTIM 1000

#define USEONESHOTTIMER TRUE

//NEIGHBORCLOCKMONITORPERIOD in ticks
//#define NEIGHBORCLOCKMONITORPERIOD 200000 800000
#define NEIGHBORCLOCKMONITORPERIOD_MICRO (100000)
#define NEIGHBORCLOCKMONITORPERIOD (g_OMAC.m_Clock.ConvertMicroSecstoTicks(NEIGHBORCLOCKMONITORPERIOD_MICRO))
#define INITIALDELAY 100000

void OMACTestBase_ReceiveHandler (void* msg, UINT16 PacketType){
	++gOMACTestBase.rx_packet_count ;
	Message_15_4_t* packet_ptr = static_cast<Message_15_4_t*>(msg);
	UINT64 packetID;
	memcpy(&packetID,packet_ptr->GetPayload(),sizeof(UINT64));
#if OMACTestBase_PRINT_RX_PACKET_INFO
	hal_printf("\r\n OMACTestBase_RX: rx_packet_count = %llu ", gOMACTestBase.rx_packet_count);
	hal_printf("src = %u PacketID = %llu \r\n", packet_ptr->GetHeader()->src, packetID );
#endif
}

void OMACTestBase_NeighborChangeHandler (INT16 args){
	hal_printf("\r\n Neighbor Change Notification for %u neighbors!\r\n", args);

	for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		if(g_NeighborTable.Neighbor[i].IsAvailableForUpperLayers){
//			hal_printf("MACAddress = %u IsAvailableForUpperLayers = %u NumTimeSyncMessagesSent = %u NumberOfRecordedElements = %u \r\n"
//					, g_NeighborTable.Neighbor[i].MACAddress
//					, g_NeighborTable.Neighbor[i].IsAvailableForUpperLayers
//					, g_NeighborTable.Neighbor[i].NumTimeSyncMessagesSent
//					, g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.regressgt2.NumberOfRecordedElements(g_NeighborTable.Neighbor[i].MACAddress)
//					);
			hal_printf("MAC=%u \r\n", g_NeighborTable.Neighbor[i].MACAddress);
		}

	}
}

void OMACTestBase_SendAckHandler (void* msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus){
	hal_printf("\r\n OMACTestBase_SendAckHandler: status = %u radioAckStatus = %u ", status, radioAckStatus);

	Message_15_4_t* packet_ptr = static_cast<Message_15_4_t*>(msg);
	DataMsg_t* data_msg = (DataMsg_t*) packet_ptr->GetPayload();
	UINT64 packetID;
	if(data_msg->size <= sizeof(UINT64)) memcpy(&packetID,data_msg->payload,data_msg->size);
	else memcpy(&packetID,data_msg->payload,sizeof(UINT64));

	g_NeighborTable.DeletePacket(packet_ptr);

	hal_printf(" dest = %u  PacketID = %llu rx_packet_count = %llu \r\n",packet_ptr->GetHeader()->dest, packetID,  gOMACTestBase.rx_packet_count );


}

void CMaxTSLocalClockMonitorTimerHandler(void * arg) {
	if(gOMACTestBase.CMaxTSLocalClockMonitorTimerHandler(arg)) {
		BOOL rv = gOMACTestBase.ScheduleNextLocalCLK();
	}
}

void CMaxTSNeighborClockMonitorTimerHandler(void * arg) {
	if(gOMACTestBase.CMaxTSNeighborClockMonitorTimerHandler(arg)){
		BOOL rv = gOMACTestBase.ScheduleNextNeighborCLK();
	}
}


// TIMESYNCTEST

BOOL OMACTestBase::Initialize(MACReceiveFuncPtrType rx_fptr
		, SendAckFuncPtrType sa_fptr, NeighborChangeFuncPtrType nc_fptr){

	sequence_number = 0;
	sent_packet_count = 0;
	rx_packet_count = 0;


	LocalClkPINState = true;
	NeighborClkPINState = true;

	LocalClockMonitorFrameNum = 0;
	NeighborClockMonitorFrameNum = 0;

	NeighborFound = false;

	MyAppID = 3; //pick a number less than MAX_APPS currently 4.
	//Config.Network = 138;
	Config.NeighborLivenessDelay = 620;
//	MACReceiveFuncPtrType rx_fptr = &OMACTestBase_ReceiveHandler;
	myEventHandler.SetReceiveHandler(rx_fptr);
//	SendAckFuncPtrType sa_fptr = &OMACTestBase_SendAckHandler;
	myEventHandler.SetSendAckHandler(sa_fptr);
//	NeighborChangeFuncPtrType nc_fptr =  &OMACTestBase_NeighborChangeHandler;
	myEventHandler.SetNeighborChangeHandler(nc_fptr);
	MacId = OMAC;
	VirtTimer_Initialize();

	MAC_Initialize(&myEventHandler, MacId, MyAppID, SI4468_SPI2, (void*) &Config);

	hal_printf("Initialize OMACTestBase");

	CPU_GPIO_EnableOutputPin(gOMACTestBase.m_NEIGHBORCLOCKMONITORPIN, TRUE);
	CPU_GPIO_EnableOutputPin(gOMACTestBase.m_LOCALCLOCKMONITORPIN, TRUE);
	CPU_GPIO_SetPinState(gOMACTestBase.m_NEIGHBORCLOCKMONITORPIN, FALSE);
	CPU_GPIO_SetPinState(gOMACTestBase.m_LOCALCLOCKMONITORPIN, FALSE);

	return TRUE;
}

BOOL OMACTestBase::StartTest(){
	return TRUE;
}


void OMACTestBase_Initialize(){
	BOOL ret;
	ret = gOMACTestBase.Initialize(&OMACTestBase_ReceiveHandler, &OMACTestBase_SendAckHandler, &OMACTestBase_NeighborChangeHandler);
	ret = gOMACTestBase.StartTest();
}


