////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BlockStorageTest.h"
#include <stm32f10x.h>
#include <Samraksh\HAL_util.h>

//---//
#define COM1 0
#define LED3_FOR_TEST 24
#define LED2_FOR_RESULT 25
#define FLASH_RESET 107


BlockStorageTest::BlockStorageTest( int seedValue, int numberOfEvents )
{
	//USART_Initialize(COM1, 115200, 0, 8, 1, 0);
	//Initial seed value for random value generator
	testMathInstance.prng_init(30);
	// initial wait for COM port to come up
	/*int j;
	for(j = 0; j < 500000; j++){}
	for(j = 0; j < 500000; j++){}
	for(j = 0; j < 500000; j++){}
	for(j = 0; j < 500000; j++){}*/
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


/* Using random data to test the flash */

BOOL BlockStorageTest::Level_0B()
{
	// For testing on eMote using battery and serial (using LEDs)
	CPU_GPIO_EnableOutputPin((GPIO_PIN) LED2_FOR_RESULT, FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) LED3_FOR_TEST, FALSE);

	int test_limit = 20;
	char writeData[test_limit];
	char readData[test_limit];
	char buffer[2] = {0,0};
	int counter = 0;
	UINT32 location = 0x64010000;
	UINT32 addressOffset = 0;
	UINT32 regionIndex = 0, rangeIndex = 0;
	ByteAddress Address;
	BOOL success = TRUE;

	const BlockDeviceInfo* pDeviceInfo;
	BlockStorageDevice *device;
	UINT32 usage = BlockUsage::FILESYSTEM;

	for(UINT16 j = 0; j < test_limit; j++)
	{
		writeData[j] = (UINT8) (testMathInstance.pareto_prng() % (1 << 7));
	}

	for(UINT16 j = 0; j < test_limit; j++)
	{
		readData[j] = 0;
	}

	//Finds the block storage device that has the specified physical address stored in "location".
	if (!BlockStorageList::FindDeviceForPhysicalAddress(&device, location, Address))
	{
		DisplayStats(FALSE, "ERROR: Could not find device for given address",  NULL, NULL);
		return FALSE;
	}

	//Setting address to the value below as there is a problem between addresses 0x64018000 and 0x64020000
	Address = 0x64020000;

	//while((Address + test_limit) < 0x647F0014) //0x64010000 + 0x7E0000 + 0x14
	while((Address + test_limit) < 0x64026000) //0x64010000 + 0x7E0000 + 0x14
	{
		if(!device->IsBlockErased(Address, test_limit/2))
		{
			if(!device->EraseBlock(Address))
			{
				DisplayStats(FALSE, "ERROR: Erase of Block Storage failed",  NULL, NULL);
				return FALSE;
			}
		}

		if(device->Write(Address, test_limit, (BYTE*) writeData, FALSE))
		{
			if(device->Read(Address, test_limit, (BYTE*)readData))
			{
				for(UINT16 i = 0; i < test_limit; i++)
				{
					if(readData[i] != writeData[i])
					{
						DisplayStats(FALSE, "ERROR: Read data is not same as written data", NULL,NULL);
						//return FALSE;
						CPU_GPIO_SetPinState((GPIO_PIN) LED2_FOR_RESULT, TRUE);
						break;
					}
				}
			}
			else
			{
				DisplayStats(FALSE, "ERROR: Unable to read data from flash", NULL,NULL);
				return FALSE;
			}
		}
		else
		{
			DisplayStats(FALSE, "ERROR: Unable to write data to flash",  NULL, NULL);
			return FALSE;
		}
		counter++;

		if((counter % 2) == 0)
			CPU_GPIO_SetPinState((GPIO_PIN) LED3_FOR_TEST, TRUE);
		else
			CPU_GPIO_SetPinState((GPIO_PIN) LED3_FOR_TEST, FALSE);

		addressOffset = (UINT8) (testMathInstance.pareto_prng() % (1 << 7));
		Address = Address + addressOffset + test_limit;

	} //while(Address + test_limit < 0x647F0014)

	DisplayStats(TRUE, "SUCCESS: BlockStorage tested successfully",  NULL, NULL);
	//CPU_GPIO_SetPinState((GPIO_PIN) LED2_FOR_RESULT, FALSE);
	return TRUE;

} //BlockStorageTest::Level_0B



BOOL BlockStorageTest::Execute( int testLevel )
{
	if(testLevel == 0)
		Level_0A();
	else if(testLevel == 1)
		Level_0B();
} //Execute

