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
        const uint bufferSize = 1024;
        byte[] writeBuffer = new byte[bufferSize];
        byte[] readBuffer = new byte[bufferSize];
        int range = 1011;   // 1011 + 1 + 12 (header size) gives 1024


        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            rnd = new Random();
            //readBuffer = new byte[size];
            //writeBuffer = new byte[size];

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
        public void Level_0G()
        {
            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            /* For "overallIndex" times, create "dataIndex" count of data. For each data, write random data and read it back. 
             * Then again write to the same data, thereby marking the previous version invalid. Finally delete the data. 
             * Size of the flash is: 125 * 65536 = 819200. The below test fills up the flash "overallIndex" times. */
            for (UInt32 overallIndex = 0; overallIndex < 100; ++overallIndex)
            {
                for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
                {
                    int size = rnd.Next(range) + 1;    // Because random number ranges from 0 to range. I want size to be from 1, so adding 1.
                    Debug.Print("Size is " + size);
                    DataReference data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);

                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data write successful");
                    else
                    {
                        DisplayStats(false, "Data write failure", "", 0);
                        return;
                    }

                    if (data.Read(readBuffer, 0, size) == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data read successful");
                    else
                    {
                        DisplayStats(false, "Data read failure", "", 0);
                        return;
                    }

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

                    if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data write successful");
                    else
                    {
                        DisplayStats(false, "Data write failure", "", 0);
                        return;
                    }
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);

                    Debug.Print("Read Write successful");
                    if (data.Delete() != DATASTORE_RETURN_STATUS.Success)
                    {
                        DisplayStats(false, "Delete failed", "", 0);
                        return;
                    }
                }
            }

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0G();
        }
    }
}
