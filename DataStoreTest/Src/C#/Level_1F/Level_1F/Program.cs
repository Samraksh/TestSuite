using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

/* Test write and read APIs. Write a byte array (with sequential data - filled with numbers from 0 to 99) from a random offset. 
 * Read from that same random offset and compare against writeBuffer */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        byte[] writeBuffer;
        byte[] readBuffer;
        Type dataType;
        UInt32 size;
        uint offset = 0;
        int experimentIndex;

        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);
            
            experimentIndex = 100;
            size = 256;
            rand = new Random();
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
            dataType = typeof(byte);
        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
            while (true){
                Thread.Sleep(1000);
                if (result)
                {
                    Debug.Print("\r\nresult=PASS\r\n");
                }
                else
                {
                    Debug.Print("\r\nresult=FAIL\r\n");
                }
                Debug.Print("\r\naccuracy=" + accuracy.ToString() + "\r\n");
                Debug.Print("\r\nresultParameter1=" + resultParameter1 + "\r\n");
                Debug.Print("\r\nresultParameter2=" + resultParameter2 + "\r\n");
                Debug.Print("\r\nresultParameter3= \r\b");
                Debug.Print("\r\nresultParameter4= \r\b");
                Debug.Print("\r\nresultParameter5= \r\b");
            }
        }

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_1F()
        {
            Debug.Print("Starting test Level_1F");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
            }

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                DataAllocation data = new DataAllocation(dStore, size, dataType);

                offset = (uint)(rand.Next((int)size));
                UInt32 numData = (uint)( rand.Next( (int)(size - offset) ) );

                if (data.Write(writeBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }

                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                if (offset % 2 == 1)
                    offset = offset + sizeof(byte);

                if (data.Read(readBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Read successful", "", 0);
                else
                {
                    DisplayStats(true, "Read not successful", "", 0);
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

                DisplayStats(true, "Read Write successful", "", 0);

                Array.Clear(readBuffer, 0, readBuffer.Length);
            }
			
			if (DataStore.EraseAll() == DataStatus.Success)
                DisplayStats(true, "Datastore succesfully erased", null, 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_1F();
        }
    }
}
