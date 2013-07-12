////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GPIO.h"


//---//


GPIOTest::GPIOTest( int seedValue, int numberOfEvents )
{
	this->numberOfEvents = numberOfEvents;

	CPU_GPIO_Initialize();

	CPU_GPIO_EnableOutputPin((GPIO_PIN) GPIO_J12_PIN1 , FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) GPIO_J12_PIN2, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) GPIO_J12_PIN3, FALSE);

};

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

BOOL GPIOTest::Level_0B()
{
}




BOOL GPIOTest::Execute( int testLevel )
{
	if(testLevel == LEVEL_0A_TEST)
		Level_0A();

} //Execute

