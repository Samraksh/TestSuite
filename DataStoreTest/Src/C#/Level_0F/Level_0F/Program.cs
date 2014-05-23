using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.NonVolatileMemory;

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {

        DataStore dStore;
        Random rnd;
        //const uint size;
        int size;
        int offset;

        //static byte[] writeBuffer = new byte[size];
        //static byte[] readBuffer = new byte[size];

        byte[] writeBuffer;
        byte[] readBuffer;

        int experimentIndex;
        
        public static OutputPort resultFailure = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J11_PIN3, false);
        public static OutputPort resultRWData = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN4, false);
        public static OutputPort resultDeleteData = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN5, false);


        public DataStoreTest()
        {
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            rnd = new Random();
            offset = 0;
            size = 1024;
            writeBuffer = new byte[size];
            readBuffer = new byte[size];

            experimentIndex = 500;
            //readBuffer = new byte[size];
            //writeBuffer = new byte[size];

        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
            while (true){
				Thread.Sleep(1000);
            	if (result)
            	{
            	    Debug.Print("result=PASS\n");
            	}
            	else
            	{
            	    Debug.Print("result=FAIL\n");
            	}
            	Debug.Print("accuracy=" + accuracy.ToString());
            	Debug.Print("resultParameter1=" + resultParameter1);
            	Debug.Print("resultParameter2=" + resultParameter2);
            	Debug.Print("resultParameter3=null");
            	Debug.Print("resultParameter4=null");
            	Debug.Print("resultParameter5=null");
			}
        }

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_0F()
        {
            Debug.Print("Starting test Level_0F");

            /* For "overallIndex" times, create "dataIndex" count of data. For each data, write random data and read it back. 
             * Then again write to the same data, thereby marking the previous version invalid. Finally delete the data. 
             * Size of the flash is: 125 * 65536 = 819200. The below test fills up the flash "overallIndex" times. */

            DataReference data;

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");

            for (UInt32 overallIndex = 0; overallIndex < experimentIndex; ++overallIndex)
            {
                for (UInt32 dataIndex = 0; dataIndex < 1; ++dataIndex)
                {
                    data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);
                    rnd.NextBytes(writeBuffer);

                    if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data write successful");
                    else
                    {
                        DisplayStats(false, "Data write failure", "", 0);
                        resultRWData.Write(false);
                        resultDeleteData.Write(false);
                        resultFailure.Write(true);
                        return;
                    }

                    if (data.Read(readBuffer, offset, size) == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data read successful");
                    else
                    {
                        DisplayStats(false, "Data read failure", "", 0);
                        resultRWData.Write(false);
                        resultDeleteData.Write(false);
                        resultFailure.Write(true);
                        return;
                    }

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
                    Debug.Print("Read Write successful");
                    Array.Clear(readBuffer, 0, readBuffer.Length);
                    Array.Clear(writeBuffer, 0, writeBuffer.Length);

                    rnd.NextBytes(writeBuffer);
                    if (data.Write(writeBuffer, size) == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data re-write successful");
                    else
                    {
                        DisplayStats(false, "Data re-write failure", "", 0);
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
                        if (data.Delete() != DATASTORE_RETURN_STATUS.Success)
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
                Debug.Print("Experiment run count is " + overallIndex);
            }

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                DisplayStats(true, "Datastore succesfully erased", "", 0);
        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            dtest.Level_0F();
        }
    }
}
