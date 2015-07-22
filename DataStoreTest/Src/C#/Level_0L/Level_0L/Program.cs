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
        //const double definedFlashSize = (125.0 * 131072.0) + 5000.0;     //Size of the flash is: 125 * 131072 = 16384000. Little more than actual flash size in order to check if exception is thrown.
        const uint bufferSize = 1024;

        byte[] writeBuffer = new byte[bufferSize];
        int range = 1011;   // 1011 + 1 + 12 (header size) gives 1024
        int counter = 0;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;
        //double currentFlashSize = 0.0;
        
        public DataStoreTest()
        {
            bool eraseDataStore = true;
            Debug.Print("Initializing datastore");
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);

            rnd = new Random(813);
        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
            while (true)
            {
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

        /* Below test fills up the flash until OOM exception is thrown. */
        public void Level_0L()
        {
            try
            {
                #if (__DEBUG__)
                    Debug.Print("Start time: " + System.DateTime.Now.ToString());
                #endif

                int dStoreFreeSpace = dStore.FreeBytes;
                //while (currentFlashSize < definedFlashSize)
                while (dStoreFreeSpace > 0)
                {
                    int dataAllocationSize = rnd.Next(range) + 1;    // Because random number ranges from 0 to range. I want dataAllocationSize to be from 1, so adding 1.
                    //currentFlashSize += dataAllocationSize;

                    DataReference data = new DataReference(dStore, dataAllocationSize, ReferenceDataType.BYTE);
                    
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, dataAllocationSize) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit) {
                            #if (__DEBUG__)
                                Debug.Print("Time: " + System.DateTime.Now.ToString());
                            #endif
                            DisplayStats(false, "Data write failure", "", 0);
                            return;
                        } else {
                            #if (__DEBUG__)
                                Debug.Print("errorCounter: " + errorCounter.ToString());
                            #endif
                            continue;
                        }
                    }
                    
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);
                    dStoreFreeSpace = dStore.FreeBytes;
                    counter++;
                    if (counter % 1000 == 0)
                    {
                        #if (__DEBUG__)
                            Debug.Print("Time: " + System.DateTime.Now.ToString());
                            Debug.Print("Counter: " + counter.ToString());
                        #endif

                        //double a = (currentFlashSize / definedFlashSize) * 100;
                        double usedBytes = (double)dStore.UsedBytes;
                        double dstoreSize = (double)dStore.Size;
                        double a = (usedBytes / dstoreSize) * 100;
                        //double percFilledUp = System.Math.Round(a);
                        //Debug.Print("currentFlashSize: " + currentFlashSize.ToString());
                        Debug.Print("Flash free space: " + dStoreFreeSpace.ToString());
                        Debug.Print("% filled up: " + a.ToString());
                        //Debug.Print("% filled up: " + percFilledUp.ToString());
                    }
                }
                
                DisplayStats(false, "Data allocation exceeded flash size", "", 0);
            }
            catch (DataStoreOutOfMemoryException)
            {
                #if (__DEBUG__)
                    Debug.Print("End time: " + System.DateTime.Now.ToString());
                #endif
                //Debug.Print(ex.Message);
                DisplayStats(true, "OOM exception thrown - test Level_0L successfully completed", "", 0);
                /*bool result = ex.Message.Equals("Failure - out of memory during data allocation");
                if(result)
                    DisplayStats(true, "OOM exception thrown - test Level_0L successfully completed", "", 0);
                else
                    DisplayStats(false, "OOM exception not thrown - test Level_0L failed", "", 0);*/
                return;
            }
            catch (Exception ex)
            {
                #if (__DEBUG__)
                    Debug.Print("End time: " + System.DateTime.Now.ToString());
                #endif
                Debug.Print(ex.Message);
                DisplayStats(false, "OOM exception not thrown - test Level_0L failed", "", 0);
                return;
            }
        }


        public static void Main()
        {
            #if (__DEBUG__)
                Debug.Print("Start time: " + System.DateTime.Now.ToString());
            #endif
            DataStoreTest dtest = new DataStoreTest();
            #if (__DEBUG__)
                Debug.Print("Start time: " + System.DateTime.Now.ToString());
            #endif
            dtest.Level_0L();
        }
    }
}
