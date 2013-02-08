////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BlockStorageTest.h"


//---//
#define COM1 0

BlockStorageTest::BlockStorageTest( int seedValue, int numberOfEvents )
{
	USART_Initialize(COM1, 115200, 0, 8, 1, 0);
};

BOOL BlockStorageTest::Level_0A()
{
	// There is a 20s timeout
	INT32 timeout       = 20000;
	char readData[500];
	char c = '\0';
	int counter = 0;
	int aliveCounter = 0;
	
	USART_Write(COM1, "Alive\n" , 6);
	USART_Flush(COM1);
	BlockStorageStream stream;
	const BlockDeviceInfo* deviceInfo;
	
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
			if(!stream.Initialize(BlockUsage::DEPLOYMENT))
			{
				hal_printf(" ERROR: Could not find device for DEPLOYMENT usage \r\n");
				
			}
			else
			{
				if(!stream.IsErased(counter))
				{
					if(!stream.Erase(counter))
						hal_printf("ERROR : Erase of Block Storage failed \r\n");
				}
				if(stream.Write((UINT8 *) readData, counter))
				{
					for(UINT32 i = 0; i < counter ; i++)
					{
						if(readData[i] == 'z')
							break;
							readData[i] = 0;
					}
					//stream.Seek(0, SeekOrigin::SeekBegin);
					stream.Seek(0, (BlockStorageStream::SeekOrigin) 0);
					
					if(!stream.Read((UINT8 **) &readData, counter))
					{
						hal_printf(" ERROR : Read from Block Storage failed \r\n");
					}
					else
					{
						if(!stream.Erase(counter))
						{
							hal_printf("ERROR : Erase of Block Storage failed \r\n");
						}
					}
				}
				else
				{
					hal_printf("ERROR :  Unable to write to flash \r\n");
				}
				
			}
			USART_Write(COM1, (const char*) &readData, counter);
			USART_Flush(COM1);
			counter = 0;	
		}
			
		
	}
	return TRUE;

}

BOOL BlockStorageTest::Level_0B()
{
}




BOOL BlockStorageTest::Execute( int testLevel )
{
	if(testLevel == 0)
		Level_0A();
} //Execute

