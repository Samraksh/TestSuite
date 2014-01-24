using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.NonVolatileMemory;

/* Write lot of small data (byte) such that the count goes well beyond the count that can be stored in RAM. 
 * Then get back references to the data and verify that data read is same as data written. */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataAllocation data;
        DataAllocation[] dataRefArray;
        byte[] writeBuffer;
        byte[] readBuffer;
        Type dataType;
        UInt32 size;
        UInt16 offset = 0;
        int experimentIndex;

        UInt16 offsetIndex = 0;

        public static OutputPort erasePort = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false);
        public static OutputPort writePort = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4, false);
        public static OutputPort readPort = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN5, false);

        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);

            experimentIndex = 500;
            size = 256;
            rand = new Random();
            //data = new Data[experimentIndex];
            offsetIndex = 128; 
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
        public void Level_4A()
        {
            Debug.Print("Starting test Level_4A");

            erasePort.Write(true);
            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");
            erasePort.Write(false);

            for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
            }

            //offset = (uint)(rand.Next((int)size));
            //numData = (uint)(rand.Next((int)(size - offset)));

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                data = new DataAllocation(dStore, size, dataType);

                writePort.Write(true);
                if (data.Write(writeBuffer, 0, (uint)writeBuffer.Length) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                {
                    DisplayStats(true, "Write not successful", "", 0);
                    return;
                }
                writePort.Write(false);
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
                    readPort.Write(true);
                    if (dataRefArray[dataIndex].Read(readBuffer, 0, (uint)readBuffer.Length) == DataStatus.Success)
                        DisplayStats(true, "Read successful", "", 0);
                    else
                    {
                        DisplayStats(true, "Read not successful", "", 0);
                        return;
                    }
                    readPort.Write(false);

                    for (UInt16 rwIndex = 0; rwIndex < readBuffer.Length; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            return;
                        }
                    }

                    DisplayStats(true, "Read Write successful", "", 0);

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
                Debug.Print("Datastore succesfully erased");

        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_4A();
        }
    }
}
