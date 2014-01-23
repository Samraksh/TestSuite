using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;

        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);
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

        // Test that creates a bunch of records
        public void Level_0A()
        {
            for (UInt32 dataIndex = 0; dataIndex <= 10; ++dataIndex)
            {
                Type dataType = typeof(UInt16);
                DataAllocation data = new DataAllocation(dStore, 512, dataType);
                Debug.Print("Data created successfully");
            }

            if (DataStore.EraseAll() == DataStatus.Success){
                Debug.Print("Datastore succesfully erased");
				DisplayStats(true, "Datastore succesfully erased", null, 0);
			} else {
				DisplayStats(false, "Error: Data failed to be created successfully", null, 0);
			}
        }

        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0A();
        }
    }

}

