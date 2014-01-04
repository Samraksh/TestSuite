using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        const uint bufferSize = 1024;
        byte[] writeBuffer = new byte[bufferSize];
        byte[] readBuffer = new byte[bufferSize];
        int range = 1011;   // 1011 + 1 + 12 (header size) gives 1024


        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            rnd = new Random();
            //readBuffer = new byte[size];
            //writeBuffer = new byte[size];

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
        public void Level_0G()
        {
            /*if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");

            if (DataStore.GC() == DataStatus.Success)
                Debug.Print("Datastore succesfully garbage collected");*/

            /* For "overallIndex" times, create "dataIndex" count of data. For each data, write random data and read it back. 
             * Then again write to the same data, thereby marking the previous version invalid. Finally delete the data. 
             * Size of the flash is: 125 * 65536 = 819200. The below test fills up the flash "overallIndex" times. */
            for (UInt32 overallIndex = 0; overallIndex < 100; ++overallIndex)
            {
                for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
                {
                    uint size = (uint)rnd.Next(range) + 1;    // Because random number ranges from 0 to range. I want size to be from 1, so adding 1.
                    Debug.Print("Size is " + size);
                    Type dataType = typeof(System.UInt16);
                    Data data = new Data(dStore, size, dataType);

                    rnd.NextBytes(writeBuffer);

                    data.Write(writeBuffer, size);
                    data.Read(readBuffer);

                    for (UInt16 i = 0; i < size; i++)
                    {
                        if (readBuffer[i] != writeBuffer[i])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);

                    rnd.NextBytes(writeBuffer);
                    data.Write(writeBuffer, size);
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);

                    DisplayStats(true, "Read Write successful", "", 0);
                    if (data.Delete() != DataStatus.Success)
                    {
                        DisplayStats(false, "Delete failed", "", 0);
                        return;
                    }
                }
            }
			
			if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");
        }


        public static void Main()
        {

            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_0G();
        }
    }
}
