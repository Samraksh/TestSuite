using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

/* Test write and read APIs. Write a byte array (with sequential data - filled with numbers from 0 to 99), 
 * but read from a random index and compare against that particular index in writeBuffer */

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
            dStore = new DataStore((int)StorageType.NOR);
            experimentIndex = 100;
            size = 256;
            rand = new Random();
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
            dataType = typeof(byte);
        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
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

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_1C()
        {
            Debug.Print("Starting test Level_1C");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
            }

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                Data data = new Data(dStore, size, dataType);

                if (data.Write(writeBuffer, size) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }

                offset = (uint)(rand.Next((int)size));
                UInt32 numData = (uint)( rand.Next( (int)(size - offset) ) );
                //offset = size-1;
                //UInt32 numData = size;

                if (data.Read(readBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Read successful", "", 0);
                else
                {
                    DisplayStats(true, "Read not successful", "", 0);
                    return;
                }

                for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[rwIndex + offset])
                    {
                        DisplayStats(false, "Read Write test failed", "", 0);
                        return;
                    }
                }
                
                DisplayStats(true, "Read Write successful", "", 0);

                Array.Clear(readBuffer, 0, readBuffer.Length);
            }
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_1C();
        }
    }
}
