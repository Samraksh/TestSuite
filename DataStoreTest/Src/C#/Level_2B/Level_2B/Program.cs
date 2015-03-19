using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Testing persistence. Write a UInt16 array (with sequential data - filled with numbers from 0 to 99) using an array of data objects. 
 * Then get the same references and read back the data. Check if dataType of data retrieved is correct. */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference[] data;
        DataReference[] dataRefArray;

        UInt16[] writeBuffer;
        UInt16[] readBuffer;

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
            
                experimentIndex = 10;
                size = 256;
                rand = new Random();
                data = new DataReference[experimentIndex];
                dataRefArray = new DataReference[experimentIndex];
                readBuffer = new UInt16[size];
                writeBuffer = new UInt16[size];
                for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_2B failed", "", 0);
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
        public void Level_2B()
        {
            try
            {
                offset = rand.Next((int)size);
                numData = rand.Next((int)(size - offset));

                for (; dataIndex < experimentIndex; ++dataIndex)
                {
                    data[dataIndex] = new DataReference(dStore, size, ReferenceDataType.UINT16);
                    Debug.Print("Data created successfully");

                    if (data[dataIndex].Write(writeBuffer, offset, numData) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_2B failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    Debug.Print("Experiment run count is " + dataIndex);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Test Level_2B failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_2B();
                }
            }
            TestPersistence(offset, numData);
        }


        public void TestPersistence(int offset, int numData)
        {
            try
            {
                //int[] dataIdArray = new int[dStore.CountOfDataIds()];
                //dStore.ReadAllDataIds(dataIdArray, 0);     //Get all dataIDs into the dataIdArray.
            
                //Get the data references into dataRefArray.
                if (dStore.ReadAllDataReferences(dataRefArray, 0) != DataStoreReturnStatus.Success)
                {
                    DisplayStats(false, "ReadAllDataReferences - test Level_2B failed", "", 0);
                    return;
                }

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    if (dataRefArray[dataIndex].Read(readBuffer, offset, numData) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "Read not successful - test Level_2B failed", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed - test Level_2B failed", "", 0);
                            return;
                        }
                    }

                    Debug.Print("Read Write successful");
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    Debug.Print("Experiment run count is " + dataIndex);
                }

                DisplayStats(true, "Test Level_2B successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_2B failed", "", 0);
                return;
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print("Starting test Level_2B");
            dtest.Level_2B();
        }
    }
}
