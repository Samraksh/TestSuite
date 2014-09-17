using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;

        public DataStoreTest()
        {
            bool eraseDataStore = true;
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
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

        // Test that creates a bunch of records
        public void Level_0A()
        {
            try
            {
                for (UInt32 dataIndex = 0; dataIndex <= 10; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, 512, ReferenceDataType.UINT16);
                    Debug.Print("Data created successfully");
                }
            
                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                {
                    Debug.Print("Datastore succesfully erased");
                    DisplayStats(true, "Datastore succesfully erased", null, 0);
                }
                else
                {
                    DisplayStats(false, "Error: Data failed to be created successfully", null, 0);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
        }

        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0A();
        }
    }

}

