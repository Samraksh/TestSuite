////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BlockStorageTest.h"


//---//
#define COM1 0

BlockStorageTest::BlockStorageTest( int seedValue, int numberOfEvents )
{
	//USART_Initialize(COM1, 115200, 0, 8, 1, 0);
	// initial wait for COM port to come up
	int j;
	for(j = 0; j < 500000; j++){}
	for(j = 0; j < 500000; j++){}
	for(j = 0; j < 500000; j++){}
	for(j = 0; j < 500000; j++){}
};

BOOL BlockStorageTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
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

BOOL BlockStorageTest::Level_0A()
{
	// There is a 20s timeout
	INT32 timeout       = 20000;
	char readData[500];
	char c = '\0';
	int counter = 0;
	int aliveCounter = 0;
	
	//USART_Write(COM1, "Alive\n" , 6);
	//USART_Flush(COM1);
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
				DisplayStats(FALSE, "ERROR: Could not find device for DEPLOYMENT usage", NULL, 0);
				return FALSE;
			}
			else
			{
				if(!stream.IsErased(counter))
				{
					if(!stream.Erase(counter))
						DisplayStats(FALSE, "ERROR : Erase of Block Storage failed", NULL, 0);
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
						DisplayStats(FALSE, "ERROR : Erase of Block Storage failed", NULL, 0);
					}
					else
					{
						if(!stream.Erase(counter))
						{
							DisplayStats(FALSE, "ERROR : Erase of Block Storage failed", NULL, 0);
						}
					}
				}
				else
				{
					DisplayStats(FALSE, "ERROR :  Unable to write to flash", NULL, 0);
				}
			}
			counter = 0;	
		}
		
	}
	DisplayStats(TRUE, "Test passed", readData, 0);
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

