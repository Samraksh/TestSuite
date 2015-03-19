using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs. Write a byte array (with sequential data - filled with numbers from 0 to 99), 
 * but read from a random index and compare against that particular index in writeBuffer */

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
                rand = new Random();
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
                DisplayStats(false, "Test Level_1C failed", "", 0);
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
        public void Level_1C()
        {
            try
            {
                for (; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");
                
                    if (data.Write(writeBuffer, size) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_1C failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    offset = rand.Next((int)size);
                    int numData = rand.Next( (int)(size - offset) );
                    //offset = size-1;
                    //UInt32 numData = size;

                    if (data.Read(readBuffer, offset, numData) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "Read not successful - test Level_1C failed", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex + offset])
                        {
                            DisplayStats(false, "Read Write test failed - test Level_1C failed", "", 0);
                            return;
                        }
                    }
                
                    Debug.Print("Read Write successful");
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    Debug.Print("Experiment run count is " + dataIndex);
                }

                DisplayStats(true, "Test Level_1C successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Test Level_1C failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_1C();
                }
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print("Starting test Level_1C");
            dtest.Level_1C();
        }
    }
}
