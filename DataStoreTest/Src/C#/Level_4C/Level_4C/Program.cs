﻿using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Write lot of small data (UInt32) such that the count goes well beyond the count that can be stored in RAM. 
 * Then get back references to the data and verify that data read is same as data written. */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference data;
        DataReference[] dataRefArray;

        UInt32[] writeBuffer;
        UInt32[] readBuffer;
        
        int size;
        int offset = 0;
        int experimentIndex;
        int offsetIndex = 0;
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
            
                experimentIndex = 10000;
                size = 256;
                rand = new Random();
                //data = new Data[experimentIndex];
                offsetIndex = 128;
            
                readBuffer = new UInt32[size];
                writeBuffer = new UInt32[size];
                for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_4C failed", "", 0);
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
        public void Level_4C()
        {
            try
            {
                //offset = (uint)(rand.Next((int)size));
                //numData = (uint)(rand.Next((int)(size - offset)));

                for (; dataIndex < experimentIndex; ++dataIndex)
                {
                    data = new DataReference(dStore, size, ReferenceDataType.UINT32);
                    //Debug.Print("Data created successfully");

                    if (data.Write(writeBuffer, 0, writeBuffer.Length) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            Debug.Print("errorCounter: " + errorCounter.ToString());
                            DisplayStats(false, "Data write failure - test Level_4C failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    DisplayStats(false, "Test Level_4C failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_4C();
                }
            }
            TestPersistence();
        }


        public void TestPersistence()
        {
            try
            {
                offset = 0;
                int totalRecords = experimentIndex;
                int dataAllocationIndex = totalRecords > offsetIndex ? offsetIndex : totalRecords;

                dataRefArray = new DataReference[dataAllocationIndex];
                //int[] dataIdArray = new int[256];
                int dataIndex = 0;

                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                //if (offset % 2 == 1)
                //    offset = offset + sizeof(byte);

                while (offset < totalRecords)
                {
                    //dStore.ReadAllDataIds(dataIdArray, offset);     //Get all dataIDs into the dataIdArray.
                
                    //Get the data references into dataRefArray.
                    if (dStore.ReadAllDataReferences(dataRefArray, offset) != DataStoreReturnStatus.Success)
                    {
                        Debug.Print("errorCounter: " + errorCounter.ToString());
                        DisplayStats(false, "ReadAllDataReferences - test Level_4C failed", "", 0);
                        return;
                    }

                    while (dataIndex < dataAllocationIndex)
                    {
                        if (dataRefArray[dataIndex].Read(readBuffer, 0, readBuffer.Length) != DataStoreReturnStatus.Success)
                        {
                            errorCounter++;
                            if (errorCounter > errorLimit)
                            {
                                Debug.Print("errorCounter: " + errorCounter.ToString());
                                DisplayStats(false, "Read not successful - test Level_4C failed", "", 0);
                                return;
                            }
                            else
                            {
                                continue;
                            }
                        }

                        for (UInt16 rwIndex = 0; rwIndex < readBuffer.Length; ++rwIndex)
                        {
                            if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                            {
                                errorCounter++;
                                if (errorCounter > errorLimit)
                                {
                                    Debug.Print("errorCounter: " + errorCounter.ToString());
                                    DisplayStats(false, "Read Write test failed - test Level_4C failed", "", 0);
                                    return;
                                }
                                else
                                {
                                    continue;
                                }
                            }
                        }

                        //Debug.Print("dataIndex: " + dataIndex.ToString());

                        Array.Clear(readBuffer, 0, readBuffer.Length);
                        dataIndex++;
                    }
                    Debug.Print("offset: " + offset.ToString());
                    dataIndex = 0;
                    Array.Clear(dataRefArray, 0, dataRefArray.Length);
                    offsetIndex = (UInt16)(offsetIndex < totalRecords ? offsetIndex : totalRecords);
                    offset += offsetIndex;
                
                    dataAllocationIndex = totalRecords - offset;
                    dataAllocationIndex = dataAllocationIndex > offsetIndex ? offsetIndex : dataAllocationIndex;
                }

                Debug.Print(DateTime.Now.ToString());
                DisplayStats(true, "Test Level_4C successfully completed", "", 0);
            }
            catch (DataStoreInvalidReferenceException invRefEx)
            {
                Debug.Print(invRefEx.Message);
                DisplayStats(false, "Test Level_4C failed", "", 0);
                return;
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_4C failed", "", 0);
                return;
            }

        }


        public static void Main()
        {
            Debug.Print(DateTime.Now.ToString());
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print(DateTime.Now.ToString());
            Debug.Print("Starting test Level_4C");
            dtest.Level_4C();
        }
    }
}
