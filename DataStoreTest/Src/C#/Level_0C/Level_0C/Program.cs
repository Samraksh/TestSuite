using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        UInt16 size;
        UInt16 experimentIndex;


        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);
			
            rnd = new Random();
            experimentIndex = 10;
            size = 256;
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
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
        public void Level_0C()
        {
            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt32 dataIndex = 1; dataIndex <= experimentIndex; ++dataIndex)
            {
                Type dataType = typeof(System.UInt16);
                DataAllocation data = new DataAllocation(dStore, size, dataType);
            }
            
            if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");

            if(dStore.CountOfDataIds() > 0)
                DisplayStats(false, "DeleteAll failed", "", 0);
            else
                Debug.Print("DeleteAll succeeded");

            Type dataType1 = typeof(System.Byte);
            DataAllocation d = new DataAllocation(dStore, size, dataType1);

            rnd.NextBytes(writeBuffer);
            
            if (DataStatus.Success == d.Write(writeBuffer, size))
            {
                Debug.Print("Data write successful");
            }
            else
            {
                DisplayStats(false, "Data write failed", "", 0);
                return;
            }
            
            if (DataStatus.Success == d.Read(readBuffer, 0, size))
            {
                Debug.Print("Data read successful");
            }
            else
            {
                DisplayStats(false, "Data read failed", "", 0);
                return;
            }

            for (UInt16 i = 0; i < writeBuffer.Length; i++)
            {
                if (readBuffer[i] != writeBuffer[i])
                {
                    DisplayStats(false, "Read Write test failed. DeleteAll failed", "", 0);
                    return;
                }
            }
            Array.Clear(writeBuffer, 0, writeBuffer.Length);
            Array.Clear(readBuffer, 0, readBuffer.Length);

            Debug.Print("Read Write successful. DeleteAll succeeded");
			
			if (DataStore.EraseAll() == DataStatus.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0C();
        }
    }
}
