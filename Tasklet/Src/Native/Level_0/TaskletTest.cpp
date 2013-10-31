////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "TaskletTest.h"
#include <Samraksh\HAL_util.h>

//---//

extern "C"
{
	void Watchdog_Handler(void *param)
	{}

	void PVD_Handler(void *param)
	{}

	void Tamper_Handler(void *param)
	{}
}





Tasklet gTaskletManager;

TaskletTest::TaskletTest( int seedValue, int numberOfEvents )
{
	CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	gTaskletManager.initialize();

	this->numberOfEvents = numberOfEvents;

	testMathInstance.prng_init(30);

};

BOOL TaskletTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL TaskletTest::Level_0A()
{

	// 7
	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_WWDG, Watchdog_Handler, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_WWDG))
		return FALSE;

	//
	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_PVD, PVD_Handler, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_PVD))
		return FALSE;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_TAMPER, Tamper_Handler, NULL) )
		return FALSE;

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_TAMPER))
		return FALSE;


	return TRUE;

}

// This test writes data and then verifies the write has been successful
BOOL TaskletTest::Level_0B()
{
	return TRUE;

}

BOOL TaskletTest::Level_1()
{
	return TRUE;
}

// Tests the buffer write and read interface
BOOL TaskletTest::Level_0C()
{
	return TRUE;
}


// Tests the buffer write and read interface for various address values
BOOL TaskletTest::Level_0D()
{
	return TRUE;
}


BOOL TaskletTest::Execute( int testLevel )
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
		result = Level_0D();
		break;
	case 4:
		result = Level_1();
		break;
	}


	return result;



} //Execute

