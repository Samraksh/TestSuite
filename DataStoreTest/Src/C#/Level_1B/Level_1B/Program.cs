using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs with a UInt32 array (with sequential data - filled with numbers from 0 to 99) */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        UInt32[] writeBuffer;
        UInt32[] readBuffer;
        int size;
        int offset;
        int experimentIndex;
        static UInt32 dataIndex = 0;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                size = 256;
                offset = 0;
                experimentIndex = 100;
                readBuffer = new UInt32[size];
                writeBuffer = new UInt32[size];
                for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_1B failed", "", 0);
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
        public void Level_1B()
        {
            try
            {
                for (; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.UINT32);
                    Debug.Print("Data created successfully");
                
                    if (data.Write(writeBuffer, size) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_1B failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    if (data.Read(readBuffer, offset, size) != DataStoreReturnStatus.Success)
				    {
                        DisplayStats(false, "Read not successful - test Level_1B failed", "", 0);
					    return;
				    }

                    for (UInt16 i = 0; i < writeBuffer.Length; i++)
                    {
                        if (readBuffer[i] != writeBuffer[i])
                        {
                            DisplayStats(false, "Read Write test failed - test Level_1B failed", "", 0);
                            return;
                        }
                    }

                    Debug.Print("Read Write successful");
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    Debug.Print("Experiment run count is " + dataIndex);
                }

                DisplayStats(true, "Test Level_1B successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Test Level_1B failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_1B();
                }
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print("Starting test Level_1B");
            dtest.Level_1B();
        }
    }
}
