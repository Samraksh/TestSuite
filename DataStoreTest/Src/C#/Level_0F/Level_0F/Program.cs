using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;
using Samraksh.SPOT.NonVolatileMemory;

namespace Samraksh.SPOT.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        //const uint size;
        uint size;
        uint offset;

        //static byte[] writeBuffer = new byte[size];
        //static byte[] readBuffer = new byte[size];

        byte[] writeBuffer;
        byte[] readBuffer;
        Type dataType;

        public static OutputPort resultFailure = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN3, false);
        public static OutputPort resultRWData = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4, false);
        public static OutputPort resultDeleteData = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN5, false);


        public DataStoreTest()
        {
            dStore = new DataStore((int)StorageType.NOR);
            rnd = new Random();
            offset = 0;
            size = 8192;
            writeBuffer = new byte[size];
            readBuffer = new byte[size];
            dataType = typeof(byte);
            //readBuffer = new byte[size];
            //writeBuffer = new byte[size];

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
        public void Level_0F()
        {
            Debug.Print("Starting test Level_0F");

            if (DataStore.EraseAll() == DataStatus.Success)
                Debug.Print("Datastore succesfully erased");

            if (DataStore.DeleteAllData() == DataStatus.Success)
                Debug.Print("Datastore succesfully deleted");

            /* For "overallIndex" times, create "dataIndex" count of data. For each data, write random data and read it back. 
             * Then again write to the same data, thereby marking the previous version invalid. Finally delete the data. 
             * Size of the flash is: 125 * 65536 = 819200. The below test fills up the flash "overallIndex" times. */

            Data data;
            
            for (UInt32 overallIndex = 0; overallIndex < 10000; ++overallIndex)
            {
                for (UInt32 dataIndex = 0; dataIndex < 1; ++dataIndex)
                {

                    data = new Data(dStore, size, dataType);
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) == DataStatus.Success)
                        DisplayStats(true, "Data write successful", "", 0);
                    else
                    {
                        DisplayStats(true, "Data write failure", "", 0);
                        resultRWData.Write(false);
                        resultDeleteData.Write(false);
                        resultFailure.Write(true);
                        return;
                    }

                    data.Read(readBuffer, offset, size);

                    for (UInt16 i = 0; i < writeBuffer.Length; i++)
                    {
                        if (readBuffer[i] != writeBuffer[i])
                        {
                            DisplayStats(false, "Read Write test failed", "", 0);
                            resultRWData.Write(false);
                            resultDeleteData.Write(false);
                            resultFailure.Write(true);
                            return;
                        }
                    }
                    DisplayStats(true, "Read Write successful", "", 0);
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);

                    rnd.NextBytes(writeBuffer);
                    if (data.Write(writeBuffer, size) == DataStatus.Success)
                        DisplayStats(true, "Data re-write successful", "", 0);
                    else
                    {
                        DisplayStats(true, "Data re-write failure", "", 0);
                        resultRWData.Write(false);
                        resultDeleteData.Write(false);
                        resultFailure.Write(true);
                        return;
                    }

                    Array.Clear(writeBuffer, 0, writeBuffer.Length);

                    resultRWData.Write(true);

                    if (overallIndex % 10 == 0)
                    {
                        resultRWData.Write(false);
                    }

                    if (overallIndex % 20 == 0)
                    {
                        resultDeleteData.Write(true);
                        //System.Threading.Thread.Sleep(750);
                        if (data.Delete() != DataStatus.Success)
                        {
                            DisplayStats(false, "Delete failed", "", 0);
                            return;
                        }
                    }
                    resultDeleteData.Write(false);

                    //if (dataIndex == 5)
                    //{
                       // UInt32 retVal = Debug.GC(true); //Force GC once in a while to fix the memory leakage issue.
                       // Debug.Print("Free space collected " + retVal.ToString());
                    //}

                    
                }
            }
        }


        public static void Main()
        {

            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_0F();
        }
    }
}
