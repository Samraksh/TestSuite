using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test overwrites. Write a byte array (with sequential data - filled with numbers from 0 to 99) from offset zero till half of allocation.
 * Then overwrite from a random offset (before half of allocation). Make sure that the overwrite is handled properly.
 * Read from that same random offset and compare against writeBuffer */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        byte[] writeBuffer;
        byte[] readBuffer;
        
        int size;
        int offset = 0;
        int numData = 0;
        int experimentIndex;
        static UInt32 dataIndex = 0;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                Debug.Print("Initializing datastore");
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                experimentIndex = 100;
                size = 256;
                rand = new Random(1437);
                //rand = new Random(314);   
                //rand = new Random(108);
                readBuffer = new byte[size];
                writeBuffer = new byte[size];
                for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
                {
                    writeBuffer[writeIndex] = (byte)writeIndex;
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
            while (true){
				Thread.Sleep(1000);
            	if (result)
            	{
            	    Debug.Print("result=PASS\n");
            	}
            	else
            	{
            	    Debug.Print("result=FAIL\n");
            	}
            	Debug.Print("accuracy=" + accuracy.ToString());
            	Debug.Print("resultParameter1=" + resultParameter1);
            	Debug.Print("resultParameter2=" + resultParameter2);
            	Debug.Print("resultParameter3=null");
            	Debug.Print("resultParameter4=null");
            	Debug.Print("resultParameter5=null");
			}
        }

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_3A()
        {
            try
            {
                for (; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");
                
                    if (data.Write(writeBuffer, 0, size / 2) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_3A failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    offset = rand.Next((int)size/2);
                    // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                    // even offsets, but might want to read from odd offsets. 
                    // If below 2 lines are commented, then an exception is thrown when offset is odd.
                    if (offset % 2 == 1)
                        offset = offset + sizeof(byte);

                    numData = rand.Next((int)(size - offset));

                    if (data.Write(writeBuffer, offset, numData) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_3A failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    /*######################################################*/
                    // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                    // even offsets, but might want to read from odd offsets. 
                    /*if (offset % 2 == 1)
                        offset = offset + sizeof(byte);

                    if (data.Read(readBuffer, offset, numData) == DataStatus.Success)
                        DisplayStats(true, "Read successful", "", 0);
                    else
                    {
                        DisplayStats(true, "Read not successful", "", 0);
                        return;
                    }*/

                    /*if (numData % 2 == 1)
                        numData = numData + sizeof(byte);*/

                    /*for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }*/

                    /*######################################################*/
                    /* Read before the overwrite region and verify */
                    /* Not sure why I added the below line. It might have been before I added the change to raise an exception for a byte dataType when offset is 1.
                     * if (offset == 0)
                        offset = offset + sizeof(byte); */
                    
                    if (data.Read(readBuffer, 0, offset) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "Read before overwrite not successful - test Level_3A failed", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < offset; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed - before overwrite - test Level_3A failed", "", 0);
                            return;
                        }
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);

                    /*######################################################*/
                    /* Read the overwrite region and verify */

                    // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                    // even offsets, but might want to read from odd offsets. 
                    //if (offset % 2 == 1)
                        //offset = offset + sizeof(byte);

                    if (data.Read(readBuffer, offset, numData) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "Read overwrite region not successful - test Level_3A failed", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed - overwrite - test Level_3A failed", "", 0);
                            return;
                        }
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);

                    /*######################################################*/
                    /* Read after the overwrite region and verify */

                    // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                    // even offsets, but might want to read from odd offsets. 
                    //if (offset % 2 == 1)
                        //offset = offset + sizeof(byte);

                    if ((offset + numData + 1) <= (size / 2))
                    {
                        if (data.Read(readBuffer, (offset + numData + 1), (size / 2 - (offset + numData + 1))) != DataStoreReturnStatus.Success)
                        {
                            DisplayStats(false, "Read after overwrite not successful - test Level_3A failed", "", 0);
                            return;
                        }

                        int readIndex = 0;
                        for (UInt16 rwIndex = (UInt16)(offset + numData + 1); rwIndex < size / 2; ++rwIndex)
                        {
                            if (readBuffer[readIndex++] != writeBuffer[rwIndex])
                            {
                                DisplayStats(false, "Read Write test failed - after overwrite - test Level_3A failed", "", 0);
                                return;
                            }
                        }
                        Array.Clear(readBuffer, 0, readBuffer.Length);
                    }
                    /*######################################################*/

                    Debug.Print("Read Write successful");
                }

                DisplayStats(true, "Test Level_3A successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Test Level_3A failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_3A();
                }
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print("Starting test Level_3A");
            dtest.Level_3A();
        }
    }
}
