using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Write lot of small data (UInt32) such that the count goes well beyond the count that can be stored in RAM. 
 * Then get back references to the data and verify that data read is same as data written. */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;
        DataReference data;
        DataReference[] dataRefArray;

        UInt32[] writeBuffer;
        UInt32[] readBuffer;
        
        int size;
        int offset = 0;
        int experimentIndex;
        int offsetIndex = 0;

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                experimentIndex = 500;
                size = 256;
                rand = new Random();
                //data = new Data[experimentIndex];
                offsetIndex = 128;
            
                readBuffer = new UInt32[size];
                writeBuffer = new UInt32[size];
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
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
        public void Level_4C()
        {
            try
            {
                Debug.Print("Starting test Level_4C");

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }

                //offset = (uint)(rand.Next((int)size));
                //numData = (uint)(rand.Next((int)(size - offset)));

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    data = new DataReference(dStore, size, ReferenceDataType.UINT32);
                    Debug.Print("Data created successfully");

                    if (data.Write(writeBuffer, 0, writeBuffer.Length) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                    {
                        DisplayStats(false, "Write not successful", "", 0);
                        return;
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
            TestPersistence();
        }


        public void TestPersistence()
        {
            try
            {
                offset = 0;
                int totalRecords = experimentIndex;
                int dataAllocationIndex = totalRecords > offsetIndex ? offsetIndex : totalRecords;

                dataRefArray = new DataReference[dataAllocationIndex];
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
                        DisplayStats(false, "ReadAllDataReferences", "", 0);
                        return;
                    }

                    while (dataIndex < dataAllocationIndex)
                    {
                        if (dataRefArray[dataIndex].Read(readBuffer, 0, readBuffer.Length) == DataStoreReturnStatus.Success)
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

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    DisplayStats(true, "Datastore succesfully erased", null, 0);
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }

        }


        public static void Main()
        {
            DataStoreTest dtest = new DataStoreTest();

            dtest.Level_4C();
        }
    }
}
