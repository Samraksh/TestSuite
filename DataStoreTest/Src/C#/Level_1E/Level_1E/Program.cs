using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs. Write a UInt32 array (with sequential data - filled with numbers from 0 to 99), 
 * but read from a random offset and compare against that particular offset in writeBuffer */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        UInt32[] writeBuffer;
        UInt32[] readBuffer;
        int size;
        int offset = 0;
        int experimentIndex;

        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            experimentIndex = 100;
            size = 256;
            rand = new Random();
            readBuffer = new UInt32[size];
            writeBuffer = new UInt32[size];
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
        public void Level_1E()
        {
            Debug.Print("Starting test Level_1E");

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = writeIndex;
            }

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                DataReference data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.UINT32);

                if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                {
                    DisplayStats(false, "Write not successful", "", 0);
                    return;
                }

                offset = rand.Next((int)size);
                int numData = rand.Next((int)(size - offset));
                //offset = size-1;
                //UInt32 numData = size;

                if (data.Read(readBuffer, offset, numData) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Read successful");
                else
                {
                    DisplayStats(false, "Read not successful", "", 0);
                    return;
                }

                for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[offset + rwIndex])
                    {
                        DisplayStats(false, "Read Write test failed", "", 0);
                        return;
                    }
                }

                Debug.Print("Read Write successful");

                Array.Clear(readBuffer, 0, readBuffer.Length);

                Debug.Print("Experiment run count is " + dataIndex);
            }

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", null, 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_1E();
        }
    }
}
