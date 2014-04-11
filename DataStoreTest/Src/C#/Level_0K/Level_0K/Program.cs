﻿using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;


/* Test to verify that variables that return flash size, used and free bytes work */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        DataStore dStore;
        Random rnd;

        const int size = 256;
        static byte[] writeBuffer;
        static byte[] readBuffer = new byte[size];

        
        public DataStoreTest()
        {
            //dStore = new DataStore();     ////This gives an error as the constructor is private
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            writeBuffer = new byte[size];
            rnd = new Random();
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
        public void Level_0K()
        {
            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            Debug.Print(dStore.MaxAllocationSize.ToString());
            Debug.Print(dStore.Size.ToString());
            Debug.Print(dStore.UsedBytes.ToString());
            Debug.Print(dStore.FreeBytes.ToString());
            

            for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
            {
                DataReference data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);
                rnd.NextBytes(writeBuffer);

                if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                {
                    DisplayStats(false, "Write not successful", "", 0);
                    return;
                }

                Debug.Print(dStore.UsedBytes.ToString());
                Debug.Print(dStore.FreeBytes.ToString());

            }

            Debug.Print(dStore.MaxAllocationSize.ToString());
            Debug.Print(dStore.Size.ToString());
            Debug.Print(dStore.UsedBytes.ToString());
            Debug.Print(dStore.FreeBytes.ToString());

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0K();
        }
    }
}
