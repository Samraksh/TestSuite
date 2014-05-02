using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        const uint size = 1024;
        static byte[] writeBuffer = new byte[size];
        static byte[] readBuffer = new byte[size];

        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
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
        public void Level_0H()
        {
            /*if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");*/

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {

            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_0H();
        }
    }
}
