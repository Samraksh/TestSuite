using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{

    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        Data[] data;
        Data[] dataObj;

        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            rnd = new Random();
            readBuffer = new byte[256];
            writeBuffer = new byte[256];
            data = new Data[10];
            dataObj = new Data[10];
            /*for (UInt32 dataIndex = 0; dataIndex < 10; ++dataIndex)
            {
                Data dataTemp = new Data();
                data[dataIndex] = dataTemp;
                dataObj[dataIndex] = dataTemp;
            }*/
        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
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

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_0E()
        {
            if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");

            /*if (DataStore.GC() == DataStatus.Success)
                Debug.Print("Datastore succesfully garbage collected");*/

            Type dataType = typeof(System.UInt32);
            UInt32 size = 256;
            for (UInt32 dataIndex = 0; dataIndex < 10; ++dataIndex)
            {
                data[dataIndex] = new Data(dStore, size, dataType);
                rnd.NextBytes(writeBuffer);
                data[dataIndex].Write(writeBuffer, size);
                Array.Clear(writeBuffer, 0, writeBuffer.Length);
            }
            TestPersistence();
        }

        public void TestPersistence()
        {
            int[] dataIdArray = new int[dStore.CountOfDataIds()];
            dStore.ReadAllDataIds(dataIdArray);     //Get all dataIDs into the dataIdArray.
            Data[] dataRefArray = new Data[10];
            dStore.ReadAllDataReferences(dataRefArray, 0);      //Get the data references into dataRefArray.
            
            for (UInt32 dataIndex = 0; dataIndex < 10; ++dataIndex)
            {
                if (DataStatus.Failure == dataRefArray[dataIndex].Read(readBuffer))
                {
                    DisplayStats(false, "Read failed", "", 0);
                    return;
                }
                else
                {
                    DisplayStats(true, "Read succeeded", "", 0);
                }
                Array.Clear(readBuffer, 0, readBuffer.Length);
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0E();
        }
    }
}
