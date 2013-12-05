﻿using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;


        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            rnd = new Random();
            readBuffer = new byte[256];
            writeBuffer = new byte[256];

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
        public void Level_0A()
        {
            for (UInt32 dataIndex = 1; dataIndex <= 10; ++dataIndex)
            {
                Data data = new Data(dStore, dataIndex, 256);

                if (data.Create() != DataStatus.Success)
                {
                    DisplayStats(false, "Record Creation failed", "", 0);
                    return;
                }

            }

            DisplayStats(true, "Record creation successful", "", 0);
        }

        public void Level_0C()
        {
            /*Data d = new Data(dStore, 1, 256);

            if (d.Create() != DataStatus.Success)
            {
                if (d.GetStatus() == DataStatus.AlreadyExists)
                {
                    Debug.Print("Record already exists");

                }
                else
                {
                    DisplayStats(false, "Record Creation failed", "", 0);
                    return;
                }
            }
            rnd.NextBytes(writeBuffer);
            d.Write(writeBuffer, 256);*/

            if (DataStore.DeleteAllRecords() != (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                Debug.Print("Datastore succesfully deleted");

            if (DataStore.GC() != (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE )
                Debug.Print("Datastore succesfully garbage collected");
            
            Data d = new Data(dStore, 1, 256);

            if (d.Create() != DataStatus.Success)
            {
                if (d.GetStatus() == DataStatus.AlreadyExists)
                {
                    Debug.Print("Record already exists");

                }
                else
                {
                    DisplayStats(false, "Record Creation failed", "", 0);
                    return;
                }
            }

            rnd.NextBytes(writeBuffer);
            d.Write(writeBuffer, 256);
            d.Read(readBuffer);

            for (UInt16 i = 0; i < writeBuffer.Length; i++)
            {
                if (readBuffer[i] != writeBuffer[i])
                {
                    DisplayStats(false, "Read Write test failed. DeleteAll failed", "", 0);
                    return;
                }
            }

            DisplayStats(true, "Read Write successful. DeleteAll succeeded", "", 0);
        }


        public static void Main()
        {

            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_0A();
            dtest.Level_0C();

            /*
            dStore = new DataStore((int)StorageType.NOR);
            int retVal = dStore.LastErrorStatus();
            //int ret = dStore.CreateRecord(1, 256);
            //Debug.Print("dStore.LastErrorStatus()\n");
            Random rand = new Random();
            for (UInt32 dataIndex = 1; dataIndex <= 10; ++dataIndex)
            {
                Data data = new Data(dStore, dataIndex, 256);
                retVal = (int)data.Create();
                byte[] writeBuffer = new byte[100];
                byte[] readBuffer = new byte[100];
                rand.NextBytes(writeBuffer);
                data.Write(writeBuffer, (UInt32)writeBuffer.Length);
                data.Read(readBuffer);

                for (UInt32 compareIndex = 0; compareIndex < (UInt32)writeBuffer.Length; ++compareIndex)
                {
                    if(writeBuffer[compareIndex] != readBuffer[compareIndex])
                        throw new SystemException();
                }
            }

            //Program2 p2 = new Program2();
            //p2.method2();

            //Debug.Print(Resources.GetString(Resources.StringResources.String1));
             * */
        }
    }
}