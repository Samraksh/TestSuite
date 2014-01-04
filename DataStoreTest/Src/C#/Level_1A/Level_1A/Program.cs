using System;
using Microsoft.SPOT;
using Samraksh.SPOT.NonVolatileMemory;

/* Test write and read APIs with a UInt16 array (with sequential data - filled with numbers from 0 to 99) */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        UInt16[] writeBuffer;
        UInt16[] readBuffer;
        Type dataType;
        UInt32 size;
        uint offset;
        int experimentIndex;

        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            size = 256;
            offset = 0;
            experimentIndex = 100;
            readBuffer = new UInt16[size];
            writeBuffer = new UInt16[size];
            dataType = typeof(UInt16);
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
        public void Level_1A()
        {
            Debug.Print("Starting test Level_1A");

            for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
            {
                writeBuffer[writeIndex] = writeIndex;
            }

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                Data data = new Data(dStore, size, dataType);
                
                if (data.Write(writeBuffer, size) == DataStatus.Success)
                    DisplayStats(true, "Write successful", "", 0);
                else
                    DisplayStats(true, "Write not successful", "", 0);

                if(data.Read(readBuffer, offset, size) == DataStatus.Success)
                    DisplayStats(true, "Read successful", "", 0);
                else
                    DisplayStats(true, "Read not successful", "", 0);

                for (UInt16 i = 0; i < writeBuffer.Length; i++)
                {
                    if (readBuffer[i] != writeBuffer[i])
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

            dtest.Level_1A();
        }
    }
}
