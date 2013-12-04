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


        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            rnd = new Random();
            readBuffer = new byte[256];
            writeBuffer = new byte[256];

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
        public void Level_0D()
        {
            for (UInt32 dataIndex = 1; dataIndex <= 10; ++dataIndex)
            {
                Data data = new Data(dStore, dataIndex, 256);

                if (data.Create() != DataStatus.Success)
                {
                    if (data.GetStatus() == DataStatus.AlreadyExists)
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
                data.Write(writeBuffer, 256);

                data.Delete();
                if (DataStatus.Failure == data.Read(readBuffer))
                {
                    DisplayStats(false, "Delete test successful", "", 0);
                }
                else
                {
                    DisplayStats(false, "Delete test failed", "", 0);
                    return;
                }
            }
            
        }

        
        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0D();
        }
    }
}
