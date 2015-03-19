//#define __DEBUG__

using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        DataStore dStore;
        Random rnd;
        const uint bufferSize = 1024;
        byte[] writeBuffer = new byte[bufferSize];
        byte[] readBuffer = new byte[bufferSize];
        int range = 1011;   // 1011 + 1 + 12 (header size) gives 1024

        /* The product of below 2 variables causes the test to run between 14 and 15 minutes. Set sleep value in DataStore.tst to be equal to "900000" (15 mins).
         * Setting the sleep to say 600K or 700K causes the test to end prematurely. */
        int dataIndexLimit = 25;
        int overallIndexLimit = 25;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;


        public DataStoreTest()
        {
            bool eraseDataStore = true;
            Debug.Print("Initializing datastore");
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            rnd = new Random();
            //readBuffer = new byte[size];
            //writeBuffer = new byte[size];

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
        public void Level_0G()
        {
            try
            {
                #if (__DEBUG__)
                    Debug.Print("Start time: " + System.DateTime.Now.ToString());
                #endif

                /* For "overallIndex" times, create "dataIndex" count of data. For each data, write random data and read it back. 
                 * Then again write to the same data, thereby marking the previous version invalid. Finally delete the data. 
                 * Size of the flash is: 125 * 131072 = 16384000. The below test fills up the flash "overallIndex" times. */
                for (UInt32 overallIndex = 0; overallIndex < overallIndexLimit; ++overallIndex)
                {
                    for (UInt32 dataIndex = 0; dataIndex < dataIndexLimit; ++dataIndex)
                    {
                        int size = rnd.Next(range) + 1;    // Because random number ranges from 0 to range. I want size to be from 1, so adding 1.

                        DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                        #if (__DEBUG__)
                            Debug.Print("Size is " + size);
                            Debug.Print("Data created successfully");
                        #endif

                        rnd.NextBytes(writeBuffer);

                        if (data.Write(writeBuffer, size) != DataStoreReturnStatus.Success)
                        {
                            Array.Clear(writeBuffer, 0, writeBuffer.Length);
                            errorCounter++;
                            if (errorCounter > errorLimit)
                            {
                                #if (__DEBUG__)
                                    Debug.Print("Time: " + System.DateTime.Now.ToString());
                                #endif
                                DisplayStats(false, "Data write failure - test Level_0G failed", "", 0);
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
                        

                        if (data.Read(readBuffer, 0, size) != DataStoreReturnStatus.Success)
                        {
                            Array.Clear(readBuffer, 0, readBuffer.Length);
                            DisplayStats(false, "Data read failure - test Level_0G failed", "", 0);
                            return;
                        }

                        for (UInt16 i = 0; i < size; i++)
                        {
                            if (readBuffer[i] != writeBuffer[i])
                            {
                                Debug.Print("readBuffer[i]: " + readBuffer[i]);
                                Debug.Print("writeBuffer[i]: " + writeBuffer[i]);
                                Array.Clear(readBuffer, 0, readBuffer.Length);
                                DisplayStats(false, "Read Write test failed - test Level_0G failed", "", 0);
                                return;
                            }
                        }
                        Array.Clear(writeBuffer, 0, writeBuffer.Length);
                        Array.Clear(readBuffer, 0, readBuffer.Length);
                        rnd.NextBytes(writeBuffer);

                        if (data.Write(writeBuffer, size) != DataStoreReturnStatus.Success)
                        {
                            Array.Clear(writeBuffer, 0, writeBuffer.Length);
                            errorCounter++;
                            if (errorCounter > errorLimit)
                            {
                                DisplayStats(false, "Data write failure - test Level_0G failed", "", 0);
                                return;
                            }
                            else
                            {
                                #if (__DEBUG__)
                                    Debug.Print("errorCounter: " + errorCounter.ToString());
                                #endif
                                //continue;
                            }
                        }
                        Array.Clear(writeBuffer, 0, writeBuffer.Length);
                        if (data.Delete() != DataStoreReturnStatus.Success)
                        {
                            DisplayStats(false, "Delete failed - test Level_0G failed", "", 0);
                            return;
                        }
                        
                        #if (__DEBUG__)
                            Debug.Print("Read Write successful");
                        #endif

                        Debug.Print("Count: " + dataIndex);
                    }

                    Debug.Print("Overall count: " + overallIndex);
                }

                DisplayStats(true, "Test Level_0G successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                #if (__DEBUG__)
                    Debug.Print("End time: " + System.DateTime.Now.ToString());
                #endif
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_0G failed", "", 0);
                return;
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0G();
        }
    }
}
