using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.SPOT.NonVolatileMemory;

/* Test write and read APIs with a byte array (with random data) */

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        byte[] writeBuffer;
        byte[] readBuffer;
        Type dataType;

        public DataStoreTest()
        {
            dStore = DataStore.Instance;
            dStore.InitDataStore((int)StorageType.NOR);
			
            rnd = new Random();
            readBuffer = new byte[256];
            writeBuffer = new byte[256];
            dataType = typeof(byte);
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
        public void Level_0B()
        {
            UInt32 size = 256;
            uint offset = 0;

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt32 dataIndex = 0; dataIndex < 100; ++dataIndex)
            {
                //UInt16 did = 256;
                //DataID d = new DataID(256);
                
                DataAllocation data = new DataAllocation(dStore, size, dataType);
                //Data data1 = new Data(dStore, (DATAID)256);

                //writeBuffer.Select(c => (object)c).ToArray();
                //object[] obj = byte.Cast<object>().ToArray();
                rnd.NextBytes(writeBuffer);

                if(data.Write(writeBuffer, size) == DataStatus.Success)
                    Debug.Print("Write successful");
                else
                    DisplayStats(false, "Write not successful", "", 0);

                if (data.Read(readBuffer, offset, size) == DataStatus.Success)
                    Debug.Print("Read successful");
                else
                    DisplayStats(false, "Read not successful", "", 0);

                for (UInt16 i = 0; i < writeBuffer.Length; i++)
                {
                    if (readBuffer[i] != writeBuffer[i])
                    {
                        DisplayStats(false, "Read Write test failed", "", 0);
                        return;
                    }
                }
                writeBuffer = new byte[256];
                readBuffer = new byte[256];

                Debug.Print("Read Write successful");
            }
			
			if (DataStore.EraseAll() == DataStatus.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0B();
        }
    }
}
