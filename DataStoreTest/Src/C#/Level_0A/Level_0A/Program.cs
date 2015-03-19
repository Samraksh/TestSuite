using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        DataStore dStore;
        int dataCreationLimit = 10000;
        static UInt32 dataIndex = 0;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 5;

        public DataStoreTest()
        {
            bool eraseDataStore = true;
            Debug.Print("Initializing datastore");
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
Level_0A_Start:
            try
            {
                for (; dataIndex <= dataCreationLimit; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, 512, ReferenceDataType.UINT16);
                    Debug.Print("Data created - " + dataIndex.ToString());
                }
            
                DisplayStats(true, "Test Level_0A successfully completed", null, 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Error: Data failed to be created - test Level_0A failed", null, 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    goto Level_0A_Start;
                }
            }
        }

        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0A();
        }
    }

}

