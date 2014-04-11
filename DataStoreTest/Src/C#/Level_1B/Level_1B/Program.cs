﻿using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test write and read APIs with a UInt32 array (with sequential data - filled with numbers from 0 to 99) */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        UInt32[] writeBuffer;
        UInt32[] readBuffer;
        int size;
        int offset;

        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            size = 256;
            offset = 0;
            readBuffer = new UInt32[size];
            writeBuffer = new UInt32[size];
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
        public void Level_1B()
        {
            Debug.Print("Starting test Level_1B");

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = writeIndex;
            }

            for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
            {
                DataReference data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.UINT32);

                if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
				{
                    DisplayStats(false, "Write not successful", "", 0);
					return;
				}

                if (data.Read(readBuffer, offset, size) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Read successful");
                else
				{
                    DisplayStats(false, "Read not successful", "", 0);
					return;
				}

                for (UInt16 i = 0; i < writeBuffer.Length; i++)
                {
                    if (readBuffer[i] != writeBuffer[i])
                    {
                        DisplayStats(false, "Read Write test failed", "", 0);
                        return;
                    }
                }

                Debug.Print("Read Write successful");

                Array.Clear(readBuffer, 0, readBuffer.Length);
            }

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", null, 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_1B();
        }
    }
}
