using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs. Write a byte array (with sequential data - filled with numbers from 0 to 99) from a random offset. 
 * Read from that same random offset and compare against writeBuffer */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        byte[] writeBuffer;
        byte[] readBuffer;
        int size;
        int offset = 0;
        int experimentIndex;

        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            experimentIndex = 100;
            size = 256;
            rand = new Random();
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
        public void Level_1F()
        {
            Debug.Print("Starting test Level_1F");

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
            }

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                DataReference data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);

                offset = rand.Next((int)size);
                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                if (offset % 2 == 1)
                    offset = offset + sizeof(byte);

                int numData = rand.Next( (int)(size - offset) );

                if (data.Write(writeBuffer, offset, numData) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                {
                    DisplayStats(false, "Write not successful", "", 0);
                    return;
                }

                if (data.Read(readBuffer, offset, numData) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Read successful");
                else
                {
                    DisplayStats(false, "Read not successful", "", 0);
                    return;
                }

                /*if (numData % 2 == 1)
                    numData = numData + sizeof(byte);*/

                for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                    {
                        DisplayStats(false, "Read Write test failed", "", 0);
                        return;
                    }
                }

                Debug.Print("Read Write successful");

                Array.Clear(readBuffer, 0, readBuffer.Length);

                Debug.Print("Experiment run count is " + (dataIndex + 1));
            }

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", null, 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_1F();
        }
    }
}
