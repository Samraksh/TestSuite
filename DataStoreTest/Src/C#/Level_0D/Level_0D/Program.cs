using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

namespace Samraksh.eMote.Tests
{

    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        UInt16 size = 0;
        UInt16 experimentIndex;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;

        public DataStoreTest()
        {
            bool eraseDataStore = true;
            Debug.Print("Initializing datastore");
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            rnd = new Random();
            experimentIndex = 10;
            size = 256;
            readBuffer = new byte[size];
            writeBuffer = new byte[size];

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
        public void Level_0D()
        {
            try
            {
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
                            DisplayStats(false, "Data write failure - test Level_0D failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }

                    if (data.Delete() != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "Delete failed - test Level_0D failed", "", 0);
                        return;
                    }
                    
                    if (DataStoreReturnStatus.Failure == data.Read(readBuffer, 0, size) || DataStoreReturnStatus.InvalidReference == data.Read(readBuffer, 0, size))
                    {
                        Debug.Print("Delete test successful " + dataIndex);
                    }
                    else
                    {
                        DisplayStats(false, "Delete test failed - test Level_0D failed", "", 0);
                        return;
                    }

                    Array.Clear(writeBuffer, 0, writeBuffer.Length);
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                }

                DisplayStats(true, "Test Level_0D successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_0D failed", "", 0);
                return;
            }
        }

        
        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0D();
        }
    }
}
