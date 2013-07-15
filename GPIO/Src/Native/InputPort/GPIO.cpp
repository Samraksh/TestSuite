////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GPIO.h"


//---//

static BOOL gpioCallbackRecieved = FALSE;

void GPIO_HANDLER(GPIO_PIN Pin, BOOL PinState, void *Param)
{
	CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN3, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN3, FALSE);

	gpioCallbackRecieved = TRUE;
}


GPIOTest::GPIOTest( int seedValue, int numberOfEvents )
{
	this->numberOfEvents = numberOfEvents;

	CPU_GPIO_Initialize();

	testMathInstance.prng_init(30);

	CPU_GPIO_EnableOutputPin((GPIO_PIN) GPIO_J12_PIN3, FALSE);

};

BOOL GPIOTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");
	hal_printf("\r\nDummy\r\n");
	hal_printf("\r\nDummy\r\n");
	hal_printf("\r\nDummy\r\n");

	return TRUE;
}

BOOL GPIOTest::Level_0A()
{

	UINT16 i = 0;

	//while(i++ < this->numberOfEvents)
	while(TRUE)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN1, TRUE);

		HAL_Time_Sleep_MicroSeconds(500);

		CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN1, FALSE);
	}

	return TRUE;

}

// This test is designed to exercise the reservation scheme of the gpio
// The GPIO hal driver maintains an reservation array which stores if the pins has been reserved
// This is important to avoid resource conflicts when multiple drivers attempt to initialize the same pin
BOOL GPIOTest::ReserveTest()
{
	BOOL PinsReserved[112];
	UINT16 pin = 0;

	for(UINT16 i = 0; i < 112; i++)
	{
		PinsReserved[i] = FALSE;
	}

	for(UINT16 i = 0; i < 25; i++)
	{
		// Grab a random pin
		pin = testMathInstance.pareto_prng() % 112;

		// Check if this pin has already been generated as a potential
		// pin to be reserved
		if(!PinsReserved[pin])
		{
			PinsReserved[pin] = TRUE;

			// Attempt to reserve the pin
			if(!CPU_GPIO_ReservePin((GPIO_PIN) pin, TRUE))
			{
				DisplayStats(FALSE, "GPIO Reserve Test failed because pin reservation failed", NULL, 0);
				return FALSE;
			}

		}

	}

	// Verify if the gpio hal api pin states reflect the state of the PinsReserved array
	for(UINT16 i = 0; i < 112; i++)
	{
		if(CPU_GPIO_PinIsBusy((GPIO_PIN) i) != (BOOL) PinsReserved[i])
		{
			DisplayStats(FALSE, "GPIO pin reservation test failed", NULL, 0);
			return FALSE;
		}
	}

	// If we have reached here, the test was successful
	//DisplayStats(TRUE, "GPIO pin reservation was successful", NULL, 0);

	return TRUE;


}

BOOL GPIOTest::Level_0B()
{
	INIT_STATE_CHECK();

	if(!ReserveTest())
		return FALSE;

	// Unreserve the pins you are about to use for testing
	// Its possible that the reserve test randomly reserved some of these pins
	CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN2, FALSE);
	CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN1, FALSE);
	CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN3, FALSE);

	if(!CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN2, TRUE))
	{
		DisplayStats(FALSE, "GPIO Test failed because pin reservation failed because pin is busy", NULL, 0);
		return FALSE;
	}

	if(!CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN1, TRUE))
	{
		DisplayStats(FALSE, "GPIO Test failed because pin reservation failed because pin is busy", NULL, 0);
		return FALSE;
	}

	if(!CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN3, TRUE))
	{
		DisplayStats(FALSE, "GPIO Test failed because pin reservation failed because pin is busy", NULL, 0);
		return FALSE;
	}

	if(!CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN4, TRUE))
	{
		DisplayStats(FALSE, "GPIO Test failed because pin reservation failed because pin is busy", NULL, 0);
		return FALSE;
	}

	if(!CPU_GPIO_ReservePin((GPIO_PIN) GPIO_J12_PIN5, TRUE))
	{
		DisplayStats(FALSE, "GPIO Test failed because pin reservation failed because pin is busy", NULL, 0);
		return FALSE;
	}

	if(!CPU_GPIO_PinIsBusy((GPIO_PIN) GPIO_J12_PIN2) || !CPU_GPIO_PinIsBusy((GPIO_PIN) GPIO_J12_PIN1) || !CPU_GPIO_PinIsBusy((GPIO_PIN) GPIO_J12_PIN3) || !CPU_GPIO_PinIsBusy((GPIO_PIN) GPIO_J12_PIN4) || !CPU_GPIO_PinIsBusy((GPIO_PIN) GPIO_J12_PIN5))
	{
		DisplayStats(FALSE,"GPIO Test failed because pin reservation failed", NULL, 0);
		return FALSE;
	}

	// Test Pins J12-1, J12-2 and J12-3 on the emote
	CPU_GPIO_EnableOutputPin((GPIO_PIN) GPIO_J12_PIN1, FALSE);
	CPU_GPIO_EnableInputPin((GPIO_PIN) GPIO_J12_PIN2, FALSE, GPIO_HANDLER, GPIO_INT_EDGE_LOW, RESISTOR_DISABLED);

	UINT16 i = 0;

	while(i++ < this->numberOfEvents)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN1, TRUE);

		HAL_Time_Sleep_MicroSeconds(250);

		CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN1, FALSE);

		HAL_Time_Sleep_MicroSeconds(250);

		DID_STATE_CHANGE(gpioCallbackRecieved);

		gpioCallbackRecieved = FALSE;

	}

	i = 0;
	// Test Pins J12-1, J12-2 and J12-3 on the emote
	CPU_GPIO_EnableOutputPin((GPIO_PIN) GPIO_J12_PIN4, FALSE);
	CPU_GPIO_EnableInputPin((GPIO_PIN) GPIO_J12_PIN5, FALSE, GPIO_HANDLER, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);


	while(i++ < this->numberOfEvents)
	{
		CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN4, TRUE);

		HAL_Time_Sleep_MicroSeconds(250);

		CPU_GPIO_SetPinState((GPIO_PIN) GPIO_J12_PIN4, FALSE);

		HAL_Time_Sleep_MicroSeconds(250);

		DID_STATE_CHANGE(gpioCallbackRecieved);

		gpioCallbackRecieved = FALSE;

	}

	DisplayStats(TRUE,"GPIO OutputPort/InputPort/InterruptPort Test succeeded", NULL, 0);

	return TRUE;

}




BOOL GPIOTest::Execute( int testLevel )
{
	Level_0B();
} //Execute

