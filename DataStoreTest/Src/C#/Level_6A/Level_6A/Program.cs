﻿using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Fill up flash and read back every nth object */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference data;
        DataReference[] dataRefArray;

        byte[] writeBuffer;
        byte[] readBuffer;

        int ObjectCount = 0;
        int bufferSize;
        int offset = 0;
        int experimentIndex;
        int offsetIndex = 0;
        int jumpValue = 25;
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
                bufferSize = 1024;
                rand = new Random();
                //data = new Data[experimentIndex];
                //offsetIndex = 128;
                //offsetIndex = (UInt16)(rand.Next((int)bufferSize / 2));

                readBuffer = new byte[bufferSize];
                writeBuffer = new byte[bufferSize];
                for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
                {
                    writeBuffer[writeIndex] = (byte)(writeIndex+1);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_6A failed", "", 0);
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

        public bool StartReading()
        {
            dataRefArray = new DataReference[experimentIndex];
            offset = 1;
            offsetIndex = 1;
            dataIndex = (uint)(offset % experimentIndex);
            if (dStore.ReadAllDataReferences(dataRefArray, offset) != DataStoreReturnStatus.Success)
            {
                DisplayStats(false, "ReadAllDataReferences - test Level_6A failed", "", 0);
                return false;
            }

            while (offset < ObjectCount)
            {
                if (offsetIndex > experimentIndex)
                {
                    offsetIndex = 1;
                    if (dStore.ReadAllDataReferences(dataRefArray, offset) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "ReadAllDataReferences - test Level_6A failed", "", 0);
                        return false;
                    }
                }
                /*Debug.Print("ObjectCount is " + ObjectCount.ToString());*/
                Debug.Print("offset is " + offset.ToString());
                /*Debug.Print("offsetIndex is " + offsetIndex.ToString());
                Debug.Print("Reading data " + dataIndex.ToString());*/
                if (dataRefArray[dataIndex-1].Read(readBuffer, 0, readBuffer.Length) != DataStoreReturnStatus.Success)
                {
                    if ((offset + dataIndex-1) > ObjectCount) {
                        Debug.Print("Trying to access an object which is not present");
                        return true;
                    }
                    else {
                        DisplayStats(false, "Read not successful - test Level_6A failed", "", 0);
                        return false;
                    }
                }

                for (UInt16 rwIndex = 0; rwIndex < readBuffer.Length; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                    {
                        DisplayStats(false, "Read Write test failed - test Level_6A failed", "", 0);
                        return false;
                    }
                }

                Array.Clear(readBuffer, 0, readBuffer.Length);
                offset += jumpValue;
                offsetIndex += jumpValue;
                dataIndex = (uint)(offset % experimentIndex);
            }

            return true;
        }

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_6A()
        {
            try
            {
                int dStoreFreeSpace = dStore.FreeBytes;
                while (dStoreFreeSpace > 0)
                {
                    data = new DataReference(dStore, bufferSize, ReferenceDataType.BYTE);
                    ObjectCount++;
                    //Debug.Print("Data created successfully - " + ObjectCount.ToString());
                    //Debug.Print("dStoreFreeSpace - " + dStoreFreeSpace.ToString());

                    if (data.Write(writeBuffer, 0, writeBuffer.Length) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_6A failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    dStoreFreeSpace = dStore.FreeBytes;
                }
            }
            catch (DataStoreOutOfMemoryException ex)
            {
                Debug.Print(ex.Message);
                Debug.Print("Final data created - " + ObjectCount.ToString());
                Debug.Print("Starting to read");
                bool readResult = StartReading();
                if (readResult)
                {
                    Debug.Print(DateTime.Now.ToString());
                    DisplayStats(true, "Test Level_6A successfully completed", "", 0);
                }
                else
                {
                    DisplayStats(false, "Test Level_6A failed", "", 0);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                Debug.Print("Final data created - " + ObjectCount.ToString());
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Test Level_6A failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_6A();
                }
            }
            //TestPersistence();
        }


        /*public void TestPersistence()
        {
            try
            {
                int dataIndex = 0;
                offset = 0;
                int totalRecords = experimentIndex;
                int dataAllocationIndex = totalRecords > offsetIndex ? offsetIndex : totalRecords;

                dataRefArray = new DataReference[dataAllocationIndex];

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
                        DisplayStats(false, "ReadAllDataReferences - test Level_4F failed", "", 0);
                        return;
                    }

                    while (dataIndex < dataAllocationIndex)
                    {
                        if (dataRefArray[dataIndex].Read(readBuffer, 0, readBuffer.Length) != DataStoreReturnStatus.Success)
                        {
                            DisplayStats(false, "Read not successful - test Level_4F failed", "", 0);
                            return;
                        }

                        for (UInt16 rwIndex = 0; rwIndex < readBuffer.Length; ++rwIndex)
                        {
                            if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                            {
                                DisplayStats(false, "Read Write test failed - test Level_4F failed", "", 0);
                                return;
                            }
                        }

                        Debug.Print("Read Write successful");

                        Array.Clear(readBuffer, 0, readBuffer.Length);
                        dataIndex++;
                    }
                    dataIndex = 0;
                    Array.Clear(dataRefArray, 0, dataRefArray.Length);
                    offsetIndex = (UInt16)(rand.Next((int)bufferSize / 2));
                    offsetIndex = (UInt16)(offsetIndex < totalRecords ? offsetIndex : totalRecords);
                    dataRefArray = new DataReference[offsetIndex];
                    offset += offsetIndex;

                    dataAllocationIndex = totalRecords - offset;
                    dataAllocationIndex = dataAllocationIndex > offsetIndex ? offsetIndex : dataAllocationIndex;
                }

                DisplayStats(true, "Test Level_4F successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_4F failed", "", 0);
                return;
            }
        }*/


        public static void Main()
        {
            Debug.Print(DateTime.Now.ToString());
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print(DateTime.Now.ToString());
            Debug.Print("Starting test Level_6A");
            dtest.Level_6A();
        }
    }
}
