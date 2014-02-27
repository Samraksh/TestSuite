
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

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

#define SUCCESS 1
#define FAIL 0

#define LEVEL_0_A 0
#define LEVEL_0_B 1
#define LEVEL_1_A 2
#define LEVEL_1_B 3
#define LEVEL_2	4

class GPIO
{
	public:
		GPIO (  );
		BOOL     Execute( int testLevel );
    		BOOL 	 Level_0A();
		BOOL 	 Level_0B();
		BOOL     DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy);
};

