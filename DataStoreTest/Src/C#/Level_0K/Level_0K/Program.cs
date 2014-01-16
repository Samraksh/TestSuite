using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;


/* Test to verify that variables that return flash size, used and free bytes work */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        DataStore dStore;
        Random rnd;

        const uint size = 256;
        static byte[] writeBuffer;
        static byte[] readBuffer = new byte[size];

        Type dataType;

        public DataStoreTest()
        {
            //dStore = new DataStore();     ////This gives an error as the constructor is private
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);

            writeBuffer = new byte[size];
            dataType = typeof(byte);
            rnd = new Random();
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
        public void Level_0K()
        {
            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            Debug.Print(dStore.MaxAllocationSize.ToString());
            Debug.Print(dStore.Size.ToString());
            Debug.Print(dStore.UsedBytes.ToString());
            Debug.Print(dStore.FreeBytes.ToString());
            

            for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
            {
                DataAllocation data = new DataAllocation(dStore, size, dataType);
                rnd.NextBytes(writeBuffer);

                if (data.Write(writeBuffer, size) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }

                Debug.Print(dStore.UsedBytes.ToString());
                Debug.Print(dStore.FreeBytes.ToString());

            }

            Debug.Print(dStore.MaxAllocationSize.ToString());
            Debug.Print(dStore.Size.ToString());
            Debug.Print(dStore.UsedBytes.ToString());
            Debug.Print(dStore.FreeBytes.ToString());

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0K();
        }
    }
}
