using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs with a byte array (with random data) */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        
        UInt16 size;
        UInt16 experimentIndex;

        public DataStoreTest()
        {
            bool eraseDataStore = true;
            dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
            rnd = new Random();
            size = 256;
            experimentIndex = 10;
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
        public void Level_0B()
        {
            try
            {
                int offset = 0;

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    //UInt16 did = 256;
                    //DataID d = new DataID(256);

                    DataReference data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");
                    
                    //Data data1 = new Data(dStore, (DATAID)256);

                    //writeBuffer.Select(c => (object)c).ToArray();
                    //object[] obj = byte.Cast<object>().ToArray();
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                        DisplayStats(false, "Write not successful", "", 0);

                    if (data.Read(readBuffer, offset, size) == DataStoreReturnStatus.Success)
                        Debug.Print("Read successful");
                    else
                        DisplayStats(false, "Read not successful", "", 0);

                    for (UInt16 i = 0; i < writeBuffer.Length; i++)
                    {
                        if (readBuffer[i] != writeBuffer[i])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }
                    writeBuffer = new byte[256];
                    readBuffer = new byte[256];

                    Debug.Print("Read Write successful");
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
            dtest.Level_0B();
        }
    }
}
