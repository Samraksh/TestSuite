using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs with a UInt16 array (with sequential data - filled with numbers from 0 to 99) */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        UInt16[] writeBuffer;
        UInt16[] readBuffer;
        int size;
        int offset;
        int experimentIndex;

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                size = 256;
                offset = 0;
                experimentIndex = 100;
                readBuffer = new UInt16[size];
                writeBuffer = new UInt16[size];
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
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
        public void Level_1A()
        {
            try
            {
                Debug.Print("Starting test Level_1A");

                for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.UINT16);
                    Debug.Print("Data created successfully");
                
                    if (data.Write(writeBuffer, size) == DataStoreReturnStatus.Success)
                        DisplayStats(true, "Write successful", "", 0);
                    else
                        DisplayStats(true, "Write not successful", "", 0);

                    if (data.Read(readBuffer, offset, size) == DataStoreReturnStatus.Success)
                        DisplayStats(true, "Read successful", "", 0);
                    else
                        DisplayStats(true, "Read not successful", "", 0);

                    for (UInt16 i = 0; i < writeBuffer.Length; i++)
                    {
                        if (readBuffer[i] != writeBuffer[i])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }

                    DisplayStats(true, "Read Write successful", "", 0);

                    Array.Clear(readBuffer, 0, readBuffer.Length);

                    Debug.Print("Experiment run count is " + dataIndex);
                }

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    DisplayStats(true, "Datastore succesfully erased", null, 0);
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

            dtest.Level_1A();
        }
    }
}
