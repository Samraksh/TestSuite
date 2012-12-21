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
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 10, FALSE);
	
};

BOOL USARTTest::Level_0A()
{
	// There is a 20s timeout
	INT32 timeout       = 20000;
	char readData[100];
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
			readData[counter++] = c;
		}
		
		if((int) c == 0xff)
		{
			for(int i = 0; i < counter; i++)
			{
				USART_Write(COM1,(const char*) readData[i], 1);
				USART_Flush(COM1);
			}
			counter = 0;	
		}
			
		
	}
	return TRUE;

}

BOOL USARTTest::Level_0B()
{
}




BOOL USARTTest::Execute( int testLevel )
{
	if(testLevel == 0)
		Level_0A();
} //Execute

