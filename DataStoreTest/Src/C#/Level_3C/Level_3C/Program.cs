using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.eMote.NonVolatileMemory;

/* Test overwrites. Write a UInt32 array (with sequential data - filled with numbers from 0 to 99) from offset zero till half of allocation.
 * Then overwrite from a random offset (before half of allocation). Make sure that the overwrite is handled properly.
 * Read from that same random offset and compare against writeBuffer */

namespace Samraksh.eMote.Tests
{
    public class DataStoreTest
    {
        Random rand;
        DataStore dStore;

        UInt32[] writeBuffer;
        UInt32[] readBuffer;

        int size;
        int offset = 0;
        int numData = 0;
        int experimentIndex;

        public DataStoreTest()
        {
            try
            {
                bool eraseDataStore = true;
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                experimentIndex = 100;
                size = 256;
                rand = new Random(1437);  //used to fail at iteration 46
                //rand = new Random(314);   //used to fail at iteration 98
                //rand = new Random(108);     //used to fail at iteration 12
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
        public void Level_3C()
        {
            try
            {
                Debug.Print("Starting test Level_3C");

                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
                    Debug.Print("Datastore succesfully erased");

                for (UInt16 writeIndex = 0; writeIndex < size; ++writeIndex)
                {
                    writeBuffer[writeIndex] = writeIndex;
                }

                for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
                {
                    //Debug.Print("Exp count " + dataIndex);

                    DataReference data = new DataReference(dStore, size, ReferenceDataType.UINT32);
                    Debug.Print("Data created successfully");
                
                    if (data.Write(writeBuffer, 0, size / 2) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                    {
                        DisplayStats(false, "Write not successful", "", 0);
                        return;
                    }

                    offset = rand.Next((int)size / 2);
                    numData = rand.Next((int)(size - offset));

                    //Debug.Print("offset is " + offset);
                    //Debug.Print("numData is " + numData);

                    if (data.Write(writeBuffer, offset, numData) == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else
                    {
                        DisplayStats(false, "Write not successful", "", 0);
                        return;
                    }

                    /*######################################################*/
                    /* Read before the overwrite region and verify */
                    /* Not sure why I added the below line. It might have been before I added the change to raise an exception for a byte dataType when offset is 1.
                     * if (offset == 0)
                        offset = offset + sizeof(byte);*/

                    if (data.Read(readBuffer, 0, offset) == DataStoreReturnStatus.Success)
                        Debug.Print("Read before overwrite successful");
                    else
                    {
                        DisplayStats(false, "Read before overwrite not successful", "", 0);
                        return;
                    }

                    for (UInt16 rwIndex = 0; rwIndex < offset; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed - before overwrite", "", 0);
                            Array.Clear(readBuffer, 0, readBuffer.Length);
                            return;
                        }
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);

                    /*######################################################*/
                    /* Read the overwrite region and verify */
                    if (data.Read(readBuffer, offset, numData) == DataStoreReturnStatus.Success)
                        Debug.Print("Read overwrite region successful");
                    else
                    {
                        DisplayStats(false, "Read overwrite region not successful", "", 0);
                        return;
                    }

                    /*if (dataIndex >= 0)
                    {
                        for (int i = 0; i < numData; i++)
                        {
                            Debug.Print(readBuffer[i].ToString());
                        }
                        Debug.Print("End of readBuffer");
                        for (int i = 0; i < numData; i++)
                        {
                            Debug.Print(writeBuffer[i].ToString());
                        }
                        Debug.Print("End of writeBuffer");
                    }*/

                    for (UInt16 rwIndex = 0; rwIndex < numData; ++rwIndex)
                    {
                        if (readBuffer[rwIndex] != writeBuffer[rwIndex])
                        {
                            DisplayStats(false, "Read Write test failed - overwrite", "", 0);
                            Array.Clear(readBuffer, 0, readBuffer.Length);
                            return;
                        }
                    }
                    Array.Clear(readBuffer, 0, readBuffer.Length);

                    /*######################################################*/
                    /* Read after the overwrite region and verify */
                    if ((offset + numData + 1) <= (size / 2))
                    {
                        if (data.Read(readBuffer, (offset + numData + 1), (size / 2 - (offset + numData + 1))) == DataStoreReturnStatus.Success)
                            Debug.Print("Read after overwrite successful");
                        else
                        {
                            DisplayStats(false, "Read after overwrite not successful", "", 0);
                            return;
                        }

                        int readIndex = 0;
                        for (UInt16 rwIndex = (UInt16)(offset + numData + 1); rwIndex < size / 2; ++rwIndex)
                        {
                            if (readBuffer[readIndex++] != writeBuffer[rwIndex])
                            {
                                DisplayStats(false, "Read Write test failed - after overwrite", "", 0);
                                Array.Clear(readBuffer, 0, readBuffer.Length);
                                return;
                            }
                        }
                        Array.Clear(readBuffer, 0, readBuffer.Length);
                    }
                    /*######################################################*/

                    Debug.Print("Read Write successful");
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

            dtest.Level_3C();
        }
    }
}
