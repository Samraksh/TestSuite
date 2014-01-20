﻿using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

/* Test overwrites. Write a byte array (with sequential data - filled with numbers from 0 to 99) from offset zero till half of allocation.
 * Then overwrite from a random offset (before half of allocation). Make sure that the overwrite is handled properly.
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
        UInt32 numData = 0;
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
        public void Level_3A()
        {
            Debug.Print("Starting test Level_3A");

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
            }

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                DataAllocation data = new DataAllocation(dStore, size, dataType);

                if (data.Write(writeBuffer, 0, size/2) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }

                offset = (uint)(rand.Next((int)size/2));
                numData = (uint)(rand.Next((int)(size - offset)));

                if (data.Write(writeBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }

                /*######################################################*/
                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                /*if (offset % 2 == 1)
                    offset = offset + sizeof(byte);

                if (data.Read(readBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Read successful", "", 0);
                else
                {
                    DisplayStats(true, "Read not successful", "", 0);
                    return;
                }*/

                /*if (numData % 2 == 1)
                    numData = numData + sizeof(byte);*/

                /*for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                    {
                        DisplayStats(false, "Read Write test failed", "", 0);
                        return;
                    }
                }*/

                /*######################################################*/
                /* Read before the overwrite region and verify */
                
                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                if (offset % 2 == 1)
                    offset = offset + sizeof(byte);

                if (data.Read(readBuffer, 0, offset - 1) == DataStatus.Success)
                    DisplayStats(true, "Read before overwrite successful", "", 0);
                else
                {
                    DisplayStats(true, "Read before overwrite not successful", "", 0);
                    return;
                }

                for (UInt16 rwIndex = 0; rwIndex < offset - 1; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                    {
                        DisplayStats(false, "Read Write test failed - before overwrite", "", 0);
                        return;
                    }
                }
                Array.Clear(readBuffer, 0, readBuffer.Length);

                /*######################################################*/
                /* Read the overwrite region and verify */

                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                if (offset % 2 == 1)
                    offset = offset + sizeof(byte);

                if (data.Read(readBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Read overwrite region successful", "", 0);
                else
                {
                    DisplayStats(true, "Read overwrite region not successful", "", 0);
                    return;
                }

                for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                {
                    if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                    {
                        DisplayStats(false, "Read Write test failed - overwrite", "", 0);
                        return;
                    }
                }
                Array.Clear(readBuffer, 0, readBuffer.Length);

                /*######################################################*/
                /* Read after the overwrite region and verify */

                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                if (offset % 2 == 1)
                    offset = offset + sizeof(byte);

                if ((offset + numData + 1) <= (size / 2))
                {
                    if (data.Read(readBuffer, (offset + numData + 1), (size / 2 - (offset + numData + 1))) == DataStatus.Success)
                        DisplayStats(true, "Read after overwrite successful", "", 0);
                    else
                    {
                        DisplayStats(true, "Read after overwrite not successful", "", 0);
                        return;
                    }

                    int readIndex = 0;
                    for (UInt16 rwIndex = (UInt16)(offset + numData + 1); rwIndex < size / 2; ++rwIndex)
                    {
                        if (readBuffer[readIndex++] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed - after overwrite", "", 0);
                            return;
                        }
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                }
                /*######################################################*/

                DisplayStats(true, "Read Write successful", "", 0);

            }

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_3A();
        }
    }
}