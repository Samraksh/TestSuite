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
