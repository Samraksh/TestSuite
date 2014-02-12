////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "USARTTest.h"


//---//
#define COM1 0

USARTTest::USARTTest( int seedValue, int numberOfEvents )
{
	// Initialize the PAL USART and driver  layer
	USART_Initialize(COM1, 115200, 0, 8, 1, 0);
	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 10, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 22, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 23, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, FALSE);
};

BOOL USARTTest::Level_0A()
{
	// There is a 20s timeout
	INT32 timeout       = 20000;
	char readData[500];
	char c = '\0';
	int counter = 0;
	int aliveCounter = 0;
	
	USART_Write(COM1, "Alive\n" , 6);
	USART_Flush(COM1);
	
	while(true)
	{
		const UINT32 c_EventsMask = SYSTEM_EVENT_FLAG_COM_IN;
		
        UINT32 events = Events_MaskedRead( c_EventsMask ); if(!events) continue;
		
		if(events != 0)
        {
               Events_Clear( events );
        }
		
		if(events & SYSTEM_EVENT_FLAG_COM_IN)
		{
			USART_Read(COM1, &c, 1);
			readData[0] = c;
			USART_Write(COM1,(const char*) readData[0], 1);
			USART_Flush(COM1);
		}

		/*if(c == 'z')
		{
			for(int i = 0; i < counter; i++)
			{
				USART_Write(COM1,(const char*) readData[i], 1);
				USART_Flush(COM1);
			}
			counter = 0;	
		}*/

		
	}
	return TRUE;

}

BOOL USARTTest::Level_0B()
{
	// There is a 20s timeout
	INT32 timeout       = 20000;
	char readData[500];
	char c = '\0';
	int counter = 0;
	int aliveCounter = 0;
	BOOL reset_flag = TRUE;
	
	USART_Write(COM1, "Alive\n" , 6);
	USART_Flush(COM1);
	
	CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
	
	while(true)
	{
		const UINT32 c_EventsMask = SYSTEM_EVENT_FLAG_COM_IN;
		
        UINT32 events = Events_MaskedRead( c_EventsMask ); if(!events) continue;
		
		if(events != 0)
        {
               Events_Clear( events );
        }
		
		if(events & SYSTEM_EVENT_FLAG_COM_IN)
		{
			USART_Read(COM1, &c, 1);
			readData[counter++] = c;
		}
		
		if(c == 'z')
		{
		
			USART_Write(COM1, (const char*) &readData, counter);
		
#if 0
			for(int i = 0; i < counter; i++)
			{
				USART_Write(COM1, (const char*) &readData[i], 1);
				USART_Flush(COM1);
			}
#endif
			counter = 0;	
			reset_flag = FALSE;
		}
		
	}
	
	CPU_Reset();
	
}


BOOL USARTTest::Level_0C()
{
	// There is a 20s timeout
	INT32 timeout       = 20000;
	char readData[500];
	char c = '\0';
	int counter = 0;
	int aliveCounter = 0;
	BOOL reset_flag = TRUE;
	
	USART_Write(COM1, "Alive\n" , 6);
	USART_Flush(COM1);
	
	CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
	
	while(true)
	{
		const UINT32 c_EventsMask = SYSTEM_EVENT_FLAG_COM_IN;
		
        UINT32 events = Events_MaskedRead( c_EventsMask ); if(!events) continue;
		//UINT32 events = ::Events_WaitForEvents( c_EventsMask, timeout );
		
		if(events != 0)
        {
               Events_Clear( events );
        }
		
		if(events & SYSTEM_EVENT_FLAG_COM_IN)
		{
			USART_Read(COM1, &c, 1);
			readData[counter++] = c;
		}
		
		if(c == 'z')
		{
		
			USART_Write(COM1, (const char*) &readData, counter);
		
#if 0
			for(int i = 0; i < counter; i++)
			{
				USART_Write(COM1, (const char*) &readData[i], 1);
				USART_Flush(COM1);
			}
#endif
			counter = 0;	
			reset_flag = FALSE;
		}
		
	}
	
	CPU_Reset();
	
}

// This test is designed to check the ability of the driver to change
// baud rates and uninitialize and then then initialize again
BOOL USARTTest::Level_0D()
{

	BOOL result;
	
	while(true)
	{
		result = USART_Uninitialize(COM1);

		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		if(result == TRUE)
		{
			if(!USART_Initialize(COM1, 57600, 0, 8, 1, 0))
			{
				CPU_GPIO_SetPinState((GPIO_PIN) 23, TRUE);
				CPU_GPIO_SetPinState((GPIO_PIN) 23, FALSE);

			}

			CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		}
		else
		{
			CPU_GPIO_SetPinState((GPIO_PIN) 22, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN) 22, FALSE);
		}

		CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

		for(int i = 0; i < 100000; i++);

	}
	
}


BOOL USARTTest::Execute( int testLevel )
{
	if(testLevel == 0)
		return Level_0A();
	else if(testLevel == 1)
		return Level_0B();
	else if(testLevel == 2)
		return Level_0C();
	else if(testLevel == 3)
		return Level_0D();
} //Execute

