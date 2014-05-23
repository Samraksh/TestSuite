////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "EventsTest.h"
#include <Samraksh\HAL_util.h>

//---//


EventsTest::EventsTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	testMathInstance.prng_init(3);

};

BOOL EventsTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	while(true){
		hal_printf("result=%s\n", (result) ? "PASS":"FAIL");
		hal_printf("accuracy=%d\n", accuracy);
		hal_printf("resultParameter1=%s\n", resultParameter1);
		hal_printf("resultParameter2=%s\n", resultParameter2);
		hal_printf("resultParameter3=null\n");
		hal_printf("resultParameter4=null\n");
		hal_printf("resultParameter5=null\n");
		HAL_Time_Sleep_MicroSeconds(1000000);
	}
	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL EventsTest::Level_0A()
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);

	UINT32 timeout =  100;

	const UINT32 c_EventsMask =  SYSTEM_EVENT_FLAG_USB_IN |
		                                        SYSTEM_EVENT_FLAG_BUTTON;
	UINT32 events;
	while(TRUE)
	{		
			CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
			events = ::Events_WaitForEvents( c_EventsMask, timeout );
			CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);
			events = ::Events_WaitForEvents( c_EventsMask, timeout );

		    if(events != 0)
		    {
		         Events_Clear( events );
		    }
	}

	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL EventsTest::Level_0B()
{
	return TRUE;
}

BOOL EventsTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL EventsTest::Level_0C()
{
	return TRUE;
}


BOOL EventsTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	case 2:
		result = Level_0C();
		break;
	case 3:
		result = Level_1();
		break;
	}


	return result;



} //Execute

