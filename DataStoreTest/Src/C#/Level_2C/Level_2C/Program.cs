using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Testing persistence. Write a UInt32 array (with sequential data - filled with numbers from 0 to 99) using an array of data objects. 
 * Then get the same references and read back the data. Check if dataType of data retrieved is correct. */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference[] data;
        DataReference[] dataRefArray;

        UInt32[] writeBuffer;
        UInt32[] readBuffer;

        int size;
        int offset = 0;
        int numData = 0;
        int experimentIndex;

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                experimentIndex = 10;
                size = 256;
                rand = new Random();
                data = new DataReference[experimentIndex];
                dataRefArray = new DataReference[experimentIndex];
                readBuffer = new UInt32[size];
                writeBuffer = new UInt32[size];
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
        public void Level_2C()
        {
            try
            {
                Debug.Print("Starting test Level_2C");

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }

                offset = rand.Next((int)size);
                numData = rand.Next((int)(size - offset));

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    data[dataIndex] = new DataReference(dStore, size, ReferenceDataType.UINT32);
                    Debug.Print("Data created successfully");

                    if (data[dataIndex].Write(writeBuffer, offset, numData) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                    {
                        DisplayStats(false, "Write not successful", "", 0);
                        return;
                    }
                    Debug.Print("Experiment run count is " + dataIndex);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
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
                    DisplayStats(false, "ReadAllDataReferences", "", 0);
                    return;
                }

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    if (dataRefArray[dataIndex].Read(readBuffer, offset, numData) == DataStoreReturnStatus.Success)
                        Debug.Print("Read successful");
                    else
                    {
                        DisplayStats(false, "Read not successful", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }

                    Debug.Print("Read Write successful");

                    Array.Clear(readBuffer, 0, readBuffer.Length);

                    Debug.Print("Experiment run count is " + dataIndex);
                }

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    DisplayStats(true, "Datastore succesfully erased", null, 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_2C();
        }
    }
}
