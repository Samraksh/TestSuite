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
            if (DataStore.DeleteAllRecords() != (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                Debug.Print("Datastore succesfully deleted");

            if (DataStore.GC() != (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                Debug.Print("Datastore succesfully garbage collected");

            for (UInt32 dataIndex = 1; dataIndex <= 10; ++dataIndex)
            {
                data[dataIndex - 1] = new Data(dStore, dataIndex, 256);

                if (data[dataIndex - 1].Create() != DataStatus.Success)
                {
                    if (data[dataIndex - 1].GetStatus() == DataStatus.AlreadyExists)
                    {
                        Debug.Print("Record already exists");
                    }
                    else
                    {
                        DisplayStats(false, "Record Creation failed", "", 0);
                        return;
                    }
                }
                DisplayStats(true, "Record creation successful", "", 0);

                rnd.NextBytes(writeBuffer);
                data[dataIndex - 1].Write(writeBuffer, 256);
            }
            TestPersistence();
        }

        public void TestPersistence()
        {
            int[] recIdArray = new int[256];
            dStore.ReadAllRecordIDs(recIdArray);
            //Data[] dataObj;
            //recIdArray.Length
            for (UInt32 dataIndex = 0; dataIndex < 10; ++dataIndex)
            {
                uint recordId = (uint)recIdArray[dataIndex];
                //dataObj[dataIndex] = new Data(dStore, recordId, 256);
                dataObj[dataIndex] = data[dataIndex];
                
                if (DataStatus.Failure == dataObj[dataIndex].Read(readBuffer))
                {
                    DisplayStats(false, "Read failed", "", 0);
                    return;
                }
                else
                {
                    DisplayStats(true, "Read succeeded", "", 0);
                }
                readBuffer = new byte[256];
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0E();
        }
    }
}
