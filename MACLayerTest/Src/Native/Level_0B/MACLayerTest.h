////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"


#define LEVEL_0A 0
#define LEVEL_0B 1
#define LEVEL_0C 2
#define LEVEL_1  3

#define INIT_STATE_CHECK_TEST()				UINT16 poll_counter, trx_status;   \
											UINT32 failure;						\

#define DID_STATE_CHANGE_TEST(x,y)				poll_counter = 0;               \
										do{ 							\
											if(poll_counter == 0xfffe)    \
											{  								\
												DisplayStats(FALSE,y, NULL,NULL); \
												return FALSE; 				\
											} 								\
											poll_counter++; 				\
										  }while(TRUE == x);							\

#define MONITOR_FAILURE(x)				poll_counter = 0;               \
										do{ 							\
											if(poll_counter == 0xfffe)    \
											{  								\
												failure++;				\
											} 								\
											poll_counter++; 				\
										  }while(TRUE == x);							\

class MACLayerTest
{

	int numberOfEvents;


	testMath testMathInstance;

public:

	static BOOL SendAckPending;
	static BOOL ResponsePending;

	static UINT8 MacID;

//--//
    MACLayerTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     Level_0C();

	BOOL 	 Level_1();

	BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

};

BOOL MACLayerTest::SendAckPending = FALSE;
BOOL MACLayerTest::ResponsePending =FALSE;
UINT8 MACLayerTest::MacID = 0;




