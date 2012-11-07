////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GPIO.h"
#include "Parameters.h"

/*class GPIO {
	public:
	GPIO(){
		CPU_GPIO_Initialize();	
	}
	BOOL Level_0A(){
		while (true){
			CPU_GPIO_SetPinState(10,FALSE);
			CPU_GPIO_SetPinState(10,TRUE);
		}
		return TRUE;
	}
	BOOL Level_0B(){
		return TRUE;
	}
	BOOL Execute( int testLevel) {
		if (testLevel == LEVEL_0_A){
			// Indicates the start of the test
			CPU_GPIO_EnableOutputPin(9,TRUE);

			// Configure Pin 2 as the pin of interest
			CPU_GPIO_EnableOutputPin(10,FALSE);
		}
		return TRUE;
	}
}
*/

GPIO::GPIO(  )
{
	CPU_GPIO_Initialize();	
}

BOOL GPIO::Level_0A()
{
	while (true){
		CPU_GPIO_SetPinState(10,FALSE);
		CPU_GPIO_SetPinState(10,TRUE);
	}
	return TRUE;

}

BOOL GPIO::Level_0B()
{
}




BOOL GPIO::Execute( int testLevel )
{
	if (testLevel == LEVEL_0_A){
		// Indicates the start of the test
		CPU_GPIO_EnableOutputPin(9,TRUE);
		CPU_GPIO_SetPinState(9, TRUE);

		// Configure Pin 2 as the pin of interest
		CPU_GPIO_EnableOutputPin(10,TRUE);

		Level_0A();
	}
} //Execute

