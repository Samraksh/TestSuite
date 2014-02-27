using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.NonVolatileMemory;

/* This test is to check if DataStore initialization works. Also checks if data rewrite works or not. Keep writing and rewriting random amounts of data. */
 

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference data;
        DataReference[] dataRefArray;
        byte[] writeBuffer;
        byte[] readBuffer;
        Type dataType;
        int size;
        int experimentIndex;

        
        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            //dStore.InitDataStore((int)StorageType.NOR);

            experimentIndex = 8000;
            size = 2;
            rand = new Random();
            //data = new Data[experimentIndex];
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
            dataType = typeof(byte);
        }

        public static void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
            while (true)
            {
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
        public void Level_4A()
        {
            Debug.Print("Starting test Level_4A");

            /*if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");*/
            
            for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
            }

            //offset = (uint)(rand.Next((int)size));
            //numData = (uint)(rand.Next((int)(size - offset)));

            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                size = rand.Next(828) + 2;
                data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);

                if (data.Write(writeBuffer, 0, writeBuffer.Length) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                {
                    DisplayStats(false, "Write not successful", "", 0);
                    return;
                }

                if (data.Write(writeBuffer, 0, writeBuffer.Length) == DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Write successful");
                else
                {
                    DisplayStats(false, "Write not successful", "", 0);
                    return;
                }
            }

            /*UInt16 offset = 0;
            int totalRecords = dStore.CountOfDataIds();
            int dataAllocationIndex = totalRecords > experimentIndex ? experimentIndex : totalRecords;
            dataRefArray = new DataReference[dataAllocationIndex + 5];
            dStore.ReadAllDataReferences(dataRefArray, offset);      //Get the data references into dataRefArray.
            foreach (DataReference dataRef in dataRefArray)
            {
                if (dataRef == null)
                    Debug.Print("Null dataRef");
            }*/
        }


        
        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_4A();
            //DisplayStats(true, "COM port test", null, 0);
        }
    }
}
