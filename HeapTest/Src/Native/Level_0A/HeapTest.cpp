////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "HeapTest.h"
//#include <stdlib.h>		//for rand()
//#include <D:\AnanthAtSamraksh\MF\MicroFrameworkPK_v4_3\DeviceCode\pal\OpenSSL\OpenSSL_1_0_0\crypto\rand\rand.h>


//---//

HeapTest::HeapTest( int seedValue, int numberOfEvents )
{
	/*CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 31, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 8, TRUE);*/

	//srand(1729);
};

BOOL HeapTest::Level_0A()
{
	int num = 0;
	hal_printf("-- Inserting -- \n");
	for(int i = 1; i <= 10; i++)
	{
		for(int j = i * 10; j > (i * 10) - 10; j--)
		{
			//num = rand() % 100 + 1;
			hal_printf("%d \n", j);
			timerQueue.Insert(j);
		}
	}

	hal_printf("\n \n");

	hal_printf("-- Extracting -- \n");
	for(int i = 0; i < 100; i++)
	{
		num = timerQueue.ExtractTop();
		hal_printf("%d \n", num);
	}

	return TRUE;
}


BOOL HeapTest::Level_0B()
{
	for(int i = 10; i >= 1; i--)
	{
		for(int j = i * 10; j > (i * 10) - 10; j--)
		{
			hal_printf("Inserting -- %d \n", j);
			timerQueue.Insert(j);
			hal_printf("top number is %d \n", timerQueue.PeekTop());
		}
	}

	return TRUE;
}



BOOL HeapTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
} //Execute

