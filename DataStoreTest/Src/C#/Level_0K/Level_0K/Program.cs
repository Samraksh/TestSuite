using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;


/* Test to verify that variables that return flash size, used and free bytes work */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        DataStore dStore;
        Random rnd;

        const int size = 256;
        static byte[] writeBuffer;
        static byte[] readBuffer = new byte[size];

        int experimentIndex;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;

        
        public DataStoreTest()
        {
            bool eraseDataStore = true;
            //dStore = new DataStore();     ////This gives an error as the constructor is private
            Debug.Print("Initializing datastore");
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
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
        public void Level_0K()
        {
            try
            {
                Debug.Print(dStore.MaxAllocationSize.ToString());
                Debug.Print(dStore.Size.ToString());
                Debug.Print(dStore.UsedBytes.ToString());
                Debug.Print(dStore.FreeBytes.ToString());


                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");
                
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) != DataStoreReturnStatus.Success)
                    {
                        Array.Clear(writeBuffer, 0, writeBuffer.Length);
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_0K failed", "", 0);
                            return;
                        }
                        else
                        {
                           continue;
                        }
                    }

                    Debug.Print(dStore.UsedBytes.ToString());
                    Debug.Print(dStore.FreeBytes.ToString());
                }

                Debug.Print(dStore.MaxAllocationSize.ToString());
                Debug.Print(dStore.Size.ToString());
                Debug.Print(dStore.UsedBytes.ToString());
                Debug.Print(dStore.FreeBytes.ToString());

                DisplayStats(true, "Test Level_0K successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_0K failed", "", 0);
                return;
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0K();
        }
    }
}
