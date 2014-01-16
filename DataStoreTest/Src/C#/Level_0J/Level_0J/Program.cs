using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;


/* Test to verify that only one instance of dataStore is created */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        DataStore dStore, dStore1;
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

            ////The same instance is created below because of singleton
            dStore1 = DataStore.Instance;
            dStore1.InitDataStore((int)StorageType.NOR);

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
        public void Level_0J()
        {
            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
            {
                DataAllocation data = new DataAllocation(dStore, size, dataType);
                rnd.NextBytes(writeBuffer);

                if (data.Write(writeBuffer, size) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                    DisplayStats(true, "Write not successful", "", 0);

                ////Though a new Data object is created with dStore1, data is written to same block storage device (NOR)
                DataAllocation data1 = new DataAllocation(dStore1, size, dataType);
                rnd.NextBytes(writeBuffer);

                if (data1.Write(writeBuffer, size) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                    DisplayStats(true, "Write not successful", "", 0);
            }

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0J();
        }
    }
}
