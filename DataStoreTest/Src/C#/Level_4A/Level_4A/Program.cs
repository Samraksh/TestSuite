using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.NonVolatileMemory;

/* Write lot of small data (byte) such that the count goes well beyond the count that can be stored in RAM. 
 * Then get back references to the data and verify that data read is same as data written. */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference data;
        DataReference[] dataRefArray;
        byte[] writeBuffer;
        byte[] readBuffer;
        
        int size;
        int offset = 0;
        int experimentIndex;

        UInt16 offsetIndex = 0;
        static UInt32 dataIndex = 0;
        //Writing to the NOR flash can fail sometimes, but if retried it works. Below variables control how many times a write failure is accepted.
        int errorCounter = 0, errorLimit = 10;

        public static OutputPort erasePort = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN2, false);
        public static OutputPort writePort = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN4, false);
        public static OutputPort readPort = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN5, false);

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                Debug.Print("Initializing datastore");
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
                //dStore.InitDataStore(StorageType.NOR);
            
                experimentIndex = 500;
                size = 256;
                rand = new Random();
                //data = new Data[experimentIndex];
                offsetIndex = 128; 
                readBuffer = new byte[size];
                writeBuffer = new byte[size];
                for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
                {
                    writeBuffer[writeIndex] = (byte)writeIndex;
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_4A failed", "", 0);
                return;
            }
        }

        public static void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
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
        public void Level_4A()
        {
            try
            {
                //offset = (uint)(rand.Next((int)size));
                //numData = (uint)(rand.Next((int)(size - offset)));

                for (; dataIndex < experimentIndex; ++dataIndex)
                {
                    data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    Debug.Print("Data created successfully");

                    writePort.Write(true);
                    if (data.Write(writeBuffer, 0, writeBuffer.Length) != DataStoreReturnStatus.Success)
                    {
                        errorCounter++;
                        if (errorCounter > errorLimit)
                        {
                            DisplayStats(false, "Data write failure - test Level_4A failed", "", 0);
                            return;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    writePort.Write(false);
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                errorCounter++;
                if (errorCounter > errorLimit)
                {
                    DisplayStats(false, "Test Level_4A failed", "", 0);
                    return;
                }
                else
                {
                    Debug.Print("errorCounter: " + errorCounter.ToString());
                    Level_4A();
                }
            }
            TestPersistence();
        }


        public void TestPersistence()
        {
            try
            {
                offset = 0;
                //int totalRecords = dStore.CountOfDataIds();
                int totalRecords = experimentIndex;
                int dataAllocationIndex = totalRecords > offsetIndex ? offsetIndex : totalRecords;

                dataRefArray = new DataReference[dataAllocationIndex + 5];

                //int[] dataIdArray = new int[256];
                int dataIndex = 0;

                // This is only for testing (only byte data type) that offset is changed below. In reality the user will always write to
                // even offsets, but might want to read from odd offsets. 
                //if (offset % 2 == 1)
                //    offset = offset + sizeof(byte);

                while (offset < totalRecords)
                {
                    //dStore.ReadAllDataIds(dataIdArray, offset);     //Get all dataIDs into the dataIdArray.
                
                    //Get the data references into dataRefArray.
                    if (dStore.ReadAllDataReferences(dataRefArray, offset) != DataStoreReturnStatus.Success)
                    {
                        DisplayStats(false, "ReadAllDataReferences - test Level_4A failed", "", 0);
                        return;
                    }

                    while (dataIndex < dataAllocationIndex)
                    {
                        readPort.Write(true);
                        if (dataRefArray[dataIndex].Read(readBuffer, 0, readBuffer.Length) != DataStoreReturnStatus.Success)
                        {
                            DisplayStats(false, "Read not successful - test Level_4A failed", "", 0);
                            return;
                        }
                        readPort.Write(false);

                        for (UInt16 rwIndex = 0; rwIndex < readBuffer.Length; ++rwIndex)
                        {
                            if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                            {
                                DisplayStats(false, "Read Write test failed - test Level_4A failed", "", 0);
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

                DisplayStats(true, "Test Level_4A successfully completed", "", 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                DisplayStats(false, "Test Level_4A failed", "", 0);
                return;
            }

        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();
            Debug.Print("Starting test Level_4A");
            dtest.Level_4A();
        }
    }
}
