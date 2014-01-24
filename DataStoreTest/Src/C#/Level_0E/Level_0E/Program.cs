using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{

    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        DataAllocation[] data;
        DataAllocation[] dataObj;
        UInt16 experimentIndex;
        UInt16 size;

        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);
            
            rnd = new Random();
            experimentIndex = 10;
            size = 256;
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
            data = new DataAllocation[experimentIndex];
            dataObj = new DataAllocation[experimentIndex];
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
                    Debug.Print("\r\nresult=PASS\r\n");
                }
                else
                {
                    Debug.Print("\r\nresult=FAIL\r\n");
                }
                Debug.Print("\r\naccuracy=" + accuracy.ToString() + "\r\n");
                Debug.Print("\r\nresultParameter1=" + resultParameter1 + "\r\n");
                Debug.Print("\r\nresultParameter2=" + resultParameter2 + "\r\n");
                Debug.Print("\r\nresultParameter3= \r\b");
                Debug.Print("\r\nresultParameter4= \r\b");
                Debug.Print("\r\nresultParameter5= \r\b");
            }

        }

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_0E()
        {
            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            Type dataType = typeof(System.UInt32);
            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                data[dataIndex] = new DataAllocation(dStore, size, dataType);
                rnd.NextBytes(writeBuffer);
                data[dataIndex].Write(writeBuffer, size);
                Array.Clear(writeBuffer, 0, writeBuffer.Length);
            }
            TestPersistence();
        }

        public void TestPersistence()
        {
            ////int[] dataIdArray = new int[dStore.CountOfDataIds()];
            ////dStore.ReadAllDataIds(dataIdArray, 0);     //Get all dataIDs into the dataIdArray.
            DataAllocation[] dataRefArray = new DataAllocation[experimentIndex];
            dStore.ReadAllDataReferences(dataRefArray, 0);      //Get the data references into dataRefArray.
            
            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                if (DataStatus.Failure == dataRefArray[dataIndex].Read(readBuffer, 0, size))
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
			
			if (DataStore.EraseAll() == DataStatus.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0E();
        }
    }
}
