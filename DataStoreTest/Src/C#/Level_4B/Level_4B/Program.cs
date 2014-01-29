using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.SPOT.NonVolatileMemory;

/* Write lot of small data (UInt16) such that the count goes well beyond the count that can be stored in RAM. 
 * Then get back references to the data and verify that data read is same as data written. */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataAllocation data;
        DataAllocation[] dataRefArray;
        UInt16[] writeBuffer;
        UInt16[] readBuffer;
        Type dataType;
        UInt32 size;
        UInt16 offset = 0;
        int experimentIndex;

        UInt16 offsetIndex = 0;

        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);

            experimentIndex = 1024;
            size = 256;
            rand = new Random();
            //data = new Data[experimentIndex];
            offsetIndex = 128;
            
            readBuffer = new UInt16[size];
            writeBuffer = new UInt16[size];
            dataType = typeof(UInt16);
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
        public void Level_4B()
        {
            Debug.Print("Starting test Level_4B");

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
            {
                writeBuffer[writeIndex] = writeIndex;
            }

            //offset = (uint)(rand.Next((int)size));
            //numData = (uint)(rand.Next((int)(size - offset)));

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                data = new DataAllocation(dStore, size, dataType);

                if (data.Write(writeBuffer, 0, (uint)writeBuffer.Length) == DataStatus.Success)
                    Debug.Print("Write successful");
                else
                {
                    DisplayStats(false, "Write not successful", "", 0);
                    return;
                }
            }
            TestPersistence();
        }


        public void TestPersistence()
        {
            offset = 0;
            UInt32 totalRecords = dStore.CountOfDataIds();
            UInt32 dataAllocationIndex = totalRecords > offsetIndex ? offsetIndex : totalRecords;
            dataRefArray = new DataAllocation[dataAllocationIndex];
            //int[] dataIdArray = new int[256];
            int dataIndex = 0;

            // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
            // even offsets, but might want to read from odd offsets. 
            //if (offset % 2 == 1)
            //    offset = offset + sizeof(byte);

            while (offset < totalRecords)
            {
                //dStore.ReadAllDataIds(dataIdArray, offset);     //Get all dataIDs into the dataIdArray.
                dStore.ReadAllDataReferences(dataRefArray, offset);      //Get the data references into dataRefArray.

                while (dataIndex < dataAllocationIndex)
                {
                    if (dataRefArray[dataIndex].Read(readBuffer, 0, (uint)readBuffer.Length) == DataStatus.Success)
                        Debug.Print("Read successful");
                    else
                    {
                        DisplayStats(false, "Read not successful", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < readBuffer.Length; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }

                    Debug.Print("Read Write successful");

                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    dataIndex++;
                }
                dataIndex = 0;
                Array.Clear(dataRefArray, 0, dataRefArray.Length);
                offsetIndex = (UInt16)(offsetIndex < totalRecords ? offsetIndex : totalRecords);
                offset += offsetIndex;
                
                dataAllocationIndex = totalRecords - offset;
                dataAllocationIndex = dataAllocationIndex > offsetIndex ? offsetIndex : dataAllocationIndex;
            }

            if (DataStore.EraseAll() == DataStatus.Success)
                DisplayStats(true, "Datastore succesfully erased", null, 0);

        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_4B();
        }
    }
}
