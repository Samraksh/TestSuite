//#define __DEBUG__

using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs with a byte array (with random data) */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        
        UInt16 size;
        UInt16 experimentIndex;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;

        public DataStoreTest()
        {
            bool eraseDataStore = true;
            Debug.Print("Initializing datastore");
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            rnd = new Random();
            size = 256;
            experimentIndex = 10;
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
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
        public void Level_0B()
        {
            int offset = 0;
            try
            {
                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");
                    
                    //writeBuffer.Select(c => (object)c).ToArray();
                    //object[] obj = byte.Cast<object>().ToArray();
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) != DataStoreReturnStatus.Success) 
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_0B failed", "", 0);
                            return;
                        }
                        else
                        {
                            #if (__DEBUG__)
                                Debug.Print("errorCounter: " + errorCounter.ToString());
                            #endif
                            continue;
                        }    
                    }

                    if (data.Read(readBuffer, offset, size) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "Read not successful - test Level_0B failed", "", 0);
                        return;
                    }

                    for (UInt16 i = 0; i < writeBuffer.Length; i++)
                    {
                        if (readBuffer[i] != writeBuffer[i])
                        {
                            DisplayStats(false, "Read Write test failed - test Level_0B failed", "", 0);
                            return;
                        }
                    }
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    
                    Debug.Print("Read Write successful");
                }

                DisplayStats(true, "Test Level_0B successfully completed", null, 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_0B failed", null, 0);
                return;
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0B();
        }
    }
}
