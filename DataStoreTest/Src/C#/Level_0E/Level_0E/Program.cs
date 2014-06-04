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
        byte[] writeBuffer;
        byte[] readBuffer;
        DataReference[] data;
        DataReference[] dataObj;
        UInt16 experimentIndex;
        UInt16 size;

        public DataStoreTest()
        {
            bool eraseDataStore = false;
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            rnd = new Random();
            experimentIndex = 10;
            size = 256;
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
            data = new DataReference[experimentIndex];
            dataObj = new DataReference[experimentIndex];
            /*for (UInt32 dataIndex = 0; dataIndex < 10; ++dataIndex)
            {
                Data dataTemp = new Data();
                data[dataIndex] = dataTemp;
                dataObj[dataIndex] = dataTemp;
            }*/
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
        public void Level_0E()
        {
            try
            {
                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    data[dataIndex] = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    rnd.NextBytes(writeBuffer);
                    data[dataIndex].Write(writeBuffer, size);
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
            TestPersistence();
        }

        public void TestPersistence()
        {
            try
            {
                ////int[] dataIdArray = new int[dStore.CountOfDataIds()];
                ////dStore.ReadAllDataIds(dataIdArray, 0);     //Get all dataIDs into the dataIdArray.
                DataReference[] dataRefArray = new DataReference[experimentIndex];

                //Get the data references into dataRefArray.
                if (dStore.ReadAllDataReferences(dataRefArray, 0) != DataStoreReturnStatus.Success)
                {
                    DisplayStats(false, "ReadAllDataReferences", "", 0);
                    return;
                }

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    if (DataStoreReturnStatus.Failure == dataRefArray[dataIndex].Read(readBuffer, 0, size))
                    {
                        DisplayStats(false, "Read failed", "", 0);
                        return;
                    }
                    else
                    {
                        Debug.Print("Read succeeded");
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                }

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    DisplayStats(true, "Datastore succesfully erased", "", 0);
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
            dtest.Level_0E();
        }
    }
}
