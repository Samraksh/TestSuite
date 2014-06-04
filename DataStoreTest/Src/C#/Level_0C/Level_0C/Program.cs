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
        UInt16 size;
        UInt16 experimentIndex;


        public DataStoreTest()
        {
            bool eraseDataStore = true;
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
        public void Level_0C()
        {
            try
            {
                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt32 dataIndex = 1; dataIndex <= experimentIndex; ++dataIndex)
                {
                    DataReference data = new DataReference(dStore, size, ReferenceDataType.UINT16);
                    Debug.Print("Data created successfully");
                }

                if (dStore.DeleteAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully deleted");

                DataReference d = new DataReference(dStore, size, ReferenceDataType.BYTE);

                rnd.NextBytes(writeBuffer);

                if (DataStoreReturnStatus.Success == d.Write(writeBuffer, size))
                {
                    Debug.Print("Data write successful");
                }
                else
                {
                    DisplayStats(false, "Data write failed", "", 0);
                    return;
                }

                if (DataStoreReturnStatus.Success == d.Read(readBuffer, 0, size))
                {
                    Debug.Print("Data read successful");
                }
                else
                {
                    DisplayStats(false, "Data read failed", "", 0);
                    return;
                }

                for (UInt16 i = 0; i < writeBuffer.Length; i++)
                {
                    if (readBuffer[i] != writeBuffer[i])
                    {
                        DisplayStats(false, "Read Write test failed. DeleteAll failed", "", 0);
                        return;
                    }
                }
                Array.Clear(writeBuffer, 0, writeBuffer.Length);
                Array.Clear(readBuffer, 0, readBuffer.Length);

                Debug.Print("Read Write successful. DeleteAll succeeded");

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
            dtest.Level_0C();
        }
    }
}
