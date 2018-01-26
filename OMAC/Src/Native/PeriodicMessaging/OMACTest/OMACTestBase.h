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
#include <Samraksh/Radio_decl.h>


void OMACTestBase_ReceiveHandler (void* msg, UINT16 PacketType);
void OMACTestBase_NeighborChangeHandler (INT16 args);
void OMACTestBase_SendAckHandler (void* msg, UINT16 size, NetOpStatus status, UINT8 radioAckStatus);



typedef void (*OMACTestClockTimerHandler) (void *msg, UINT16 arg1);

class OMACTestBase{
public:
	UINT8 MyAppID;
	MACEventHandler myEventHandler;
	MACConfig Config;
	UINT8 MacId;

	BOOL Initialize(  MACReceiveFuncPtrType rx_fptr = &OMACTestBase_ReceiveHandler
					, SendAckFuncPtrType sa_fptr = &OMACTestBase_NeighborChangeHandler
					, NeighborChangeFuncPtrType nc_fptr= &OMACTestBase_SendAckHandler
					);
	BOOL StartTest();
};

OMACTest gOMACTest;

void OMACTest_Initialize();


#endif /* OMACTest_H_ */


