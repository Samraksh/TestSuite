using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

/* Testing persistence. Write a UInt32 array (with sequential data - filled with numbers from 0 to 99) using an array of data objects. 
 * Then get the same references and read back the data. Check if dataType of data retrieved is correct. */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        Data[] data;
        Data[] dataRefArray;

        UInt32[] writeBuffer;
        UInt32[] readBuffer;

        Type dataType;
        UInt32 size;
        uint offset = 0;
        UInt32 numData = 0;
        int experimentIndex;

        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            experimentIndex = 10;
            size = 256;
            rand = new Random();
            data = new Data[experimentIndex];
            dataRefArray = new Data[experimentIndex];
            readBuffer = new UInt32[size];
            writeBuffer = new UInt32[size];
            dataType = typeof(UInt32);
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
        public void Level_2C()
        {
            Debug.Print("Starting test Level_2C");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = writeIndex;
            }

            offset = (uint)(rand.Next((int)size));
            numData = (uint)(rand.Next((int)(size - offset)));

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                data[dataIndex] = new Data(dStore, size, dataType);

                if (data[dataIndex].Write(writeBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }
            }
            TestPersistence(offset, numData);
        }


        public void TestPersistence(uint offset, UInt32 numData)
        {
            int[] dataIdArray = new int[dStore.CountOfDataIds()];
            dStore.ReadAllDataIds(dataIdArray);     //Get all dataIDs into the dataIdArray.
            dStore.ReadAllDataReferences(dataRefArray, 0);      //Get the data references into dataRefArray.

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                if (dataRefArray[dataIndex].Read(readBuffer, offset, numData) == DataStatus.Success)
                    DisplayStats(true, "Read successful", "", 0);
                else
                {
                    DisplayStats(true, "Read not successful", "", 0);
                    return;
                }

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
                Debug.Print("Datastore succesfully erased");

            if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_2C();
        }
    }
}
