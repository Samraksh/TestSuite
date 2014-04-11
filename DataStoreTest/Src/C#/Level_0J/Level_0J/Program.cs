using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;


/* Test to verify that only one instance of dataStore is created */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        DataStore dStore, dStore1;
        Random rnd;

        const int size = 256;
        static byte[] writeBuffer;
        static byte[] readBuffer = new byte[size];

        Type dataType;

        public DataStoreTest()
        {
            //dStore = new DataStore();     ////This gives an error as the constructor is private
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            ////The same instance is created below because of singleton
            dStore1 = DataStore.Instance(STORAGE_TYPE.NOR);
            
            writeBuffer = new byte[size];
            dataType = typeof(byte);
            rnd = new Random();
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
        public void Level_0J()
        {
            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
            {
                DataReference data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);
                rnd.NextBytes(writeBuffer);

                if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                    DisplayStats(false, "Write not successful", "", 0);

                ////Though a new Data object is created with dStore1, data is written to same block storage device (NOR)
                DataReference data1 = new DataReference(dStore1, size, REFERENCE_DATA_TYPE.BYTE);
                rnd.NextBytes(writeBuffer);

                if (data1.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                    DisplayStats(false, "Write not successful", "", 0);
            }

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0J();
        }
    }
}
