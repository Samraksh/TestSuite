////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"

#define  GPIO_J11_PIN3 0
#define  GPIO_J11_PIN4 1
#define  GPIO_J11_PIN5 2
#define  GPIO_J11_PIN6 3
#define  GPIO_J11_PIN7 4
#define  GPIO_J11_PIN8 8
#define  GPIO_J11_PIN9 22
#define  GPIO_J11_PIN10 23
#define  GPIO_J12_PIN1 24
#define  GPIO_J12_PIN2 25
#define  GPIO_J12_PIN3 29
#define  GPIO_J12_PIN4 30
#define  GPIO_J12_PIN5 31

#define LEVEL_0A_TEST 0
#define LEVEL_0B_TEST 1

#define INIT_STATE_CHECK()				UINT16 poll_counter;

#define DID_STATE_CHANGE(x)				poll_counter = 0;               \
										do{ 							\
											if(poll_counter == 0xffff)    \
											{  								\
												DisplayStats(FALSE,"GPIO Test failed because callback timedout", NULL, 0);  \
												return FALSE; 				\
											} 								\
											poll_counter++; 				\
										  }while(!x);

class GPIOTest
{

public:

//--//
	int numberOfEvents;

	testMath testMathInstance;


    GPIOTest ( int seedValue, int numberOfEvents );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL 	 DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);

	BOOL     ReserveTest();

};


