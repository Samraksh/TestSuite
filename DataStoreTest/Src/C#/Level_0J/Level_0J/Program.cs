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

        int experimentIndex;

        public DataStoreTest()
        {
            bool eraseDataStore = true;
            //dStore = new DataStore();     ////This gives an error as the constructor is private
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            ////The same instance is created below because of singleton
            dStore1 = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            writeBuffer = new byte[size];
            rnd = new Random();
            experimentIndex = 100;
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
        public void Level_0J()
        {
            try
            {
                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");
                
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                        DisplayStats(false, "Write not successful", "", 0);

                    ////Though a new Data object is created with dStore1, data is written to same block storage device (NOR)
                    DataReference data1 = new DataReference(dStore1, size, ReferenceDataType.BYTE);
                    rnd.NextBytes(writeBuffer);

                    if (data1.Write(writeBuffer, size) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                        DisplayStats(false, "Write not successful", "", 0);

                    Debug.Print("Experiment run count is " + dataIndex);
                }

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    DisplayStats(true, "Datastore succesfully erased", "", 0);
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
            dtest.Level_0J();
        }
    }
}
