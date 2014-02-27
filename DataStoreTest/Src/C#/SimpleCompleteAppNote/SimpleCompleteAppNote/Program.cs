using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.NonVolatileMemory;


namespace SimpleCompleteAppNote
{
    public class CompleteAppNote
    {
        Random rand;
        DataStore dStore;
        DataReference data;
        DataReference[] dataRefArray;
        byte[] writeBufferByte; byte[] readBufferByte;
        UInt16[] writeBufferUInt16; UInt16[] readBufferUInt16;
        UInt32[] writeBufferUInt32; UInt32[] readBufferUInt32;
        Type dataType;
        int size;
        int experimentIndex;
        int offset = 0;


        public CompleteAppNote()
        {
            Debug.Print("Starting complete app note");
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            
            experimentIndex = 10;
            size = 2;
            rand = new Random();

            readBufferByte = new byte[size]; writeBufferByte = new byte[size];
            readBufferUInt16 = new UInt16[size]; writeBufferUInt16 = new UInt16[size];
            readBufferUInt32 = new UInt32[size]; writeBufferUInt32 = new UInt32[size];

            dataType = typeof(byte);

            //Create a flash with no data
            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");
            else
            {
                Debug.Print("Datastore could not be erased");
            }

            for (UInt16 writeIndex = 0; writeIndex < writeBufferByte.Length; ++writeIndex)
            {
                writeBufferByte[writeIndex] = (byte)writeIndex;
            }
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


        public void writeData()
        {
            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                try
                {
                    size = rand.Next(828) + 2;
                    data = new DataReference(dStore, size, REFERENCE_DATA_TYPE.BYTE);
                    DATASTORE_RETURN_STATUS retVal = data.Write(writeBufferByte, 0, writeBufferByte.Length);
                    if (retVal == DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Write successful");
                    else if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                    {
                        DisplayStats(false, "Write not successful as reference is not valid", "", 0);
                        return;
                    }
                    else
                    {
                        DisplayStats(false, "Write not successful", "", 0);
                        return;
                    }
                }
                catch (Exception ex)
                {
                    Debug.Print(" Write failed. Exception is: " + ex.Message);
                    throw;
                }
            }
        }


        void Program()
        {
            DATASTORE_RETURN_STATUS retVal;
            //Write data to flash
            try
            {
                writeData();
            }
            catch (Exception ex)
            {
                Debug.Print("Write failed. Exception is: " + ex.Message);
            }

            //Get the data references into a dataRefArray which is larger than the amount of records created.
            offset = 0;
            dataRefArray = new DataReference[experimentIndex + experimentIndex];
            if (dStore.ReadAllDataReferences(dataRefArray, offset) != DATASTORE_RETURN_STATUS.Success)
            {
                Debug.Print("ReadAllDataReferences failed");
            }

            //Read from a random reference
            int randValue = rand.Next(experimentIndex);
            try
            {
                
                Type dataType = dataRefArray[randValue].getDataReferenceType;
                int size = dataRefArray[randValue].getDataReferenceSize;

                if (dataType == typeof(byte))
                {
                    retVal = dataRefArray[randValue].Read(readBufferByte, offset, readBufferByte.Length);
                    if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                    {
                        Debug.Print("Read failed. Invalid reference");
                        throw new DataStoreException("Invalid reference");
                    }
                    else if (retVal != DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Read failed");
                    else
                        Debug.Print("Read succeeded");
                }
                else if (dataType == typeof(UInt16))
                {
                    retVal = dataRefArray[randValue].Read(readBufferUInt16, offset, readBufferUInt16.Length);
                    if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                    {
                        Debug.Print("Read failed. Invalid reference");
                        throw new DataStoreException("Invalid reference");
                    }
                    else if (retVal != DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Read failed");
                    else
                        Debug.Print("Read succeeded");
                }
                else if (dataType == typeof(UInt32))
                {
                    retVal = dataRefArray[randValue].Read(readBufferUInt32, offset, readBufferUInt32.Length);
                    if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                    {
                        Debug.Print("Read failed. Invalid reference");
                        throw new DataStoreException("Invalid reference");
                    }
                    else if (retVal != DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Read failed");
                    else
                        Debug.Print("Read succeeded");
                }
                
                
            }
            catch (Exception ex)
            {
                Debug.Print("Read failed. Exception is: " + ex.Message);
            }


            //keep reading until a null reference or end of "dataRefArray"
            int index = 0;
            while (index < experimentIndex * experimentIndex)
            {
                try
                {
                    if (dataRefArray[index] == null)
                        break;

                    Type dataType = dataRefArray[index].getDataReferenceType;

                    if (dataType == typeof(byte))
                    {
                        retVal = dataRefArray[index].Read(readBufferByte, offset, readBufferByte.Length);
                        if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                        {
                            Debug.Print("Read failed. Invalid reference");
                            throw new DataStoreException("Invalid reference");
                        }
                        else if (retVal != DATASTORE_RETURN_STATUS.Success)
                            Debug.Print("Read failed");
                        else
                            Debug.Print("Read succeeded");
                    }
                    else if (dataType == typeof(UInt16))
                    {
                        retVal = dataRefArray[index].Read(readBufferUInt16, offset, readBufferUInt16.Length);
                        if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                        {
                            Debug.Print("Read failed. Invalid reference");
                            throw new DataStoreException("Invalid reference");
                        }
                        else if (retVal != DATASTORE_RETURN_STATUS.Success)
                            Debug.Print("Read failed");
                        else
                            Debug.Print("Read succeeded");
                    }
                    else if (dataType == typeof(UInt32))
                    {
                        retVal = dataRefArray[index].Read(readBufferUInt32, offset, readBufferUInt32.Length);
                        if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                        {
                            Debug.Print("Read failed. Invalid reference");
                            throw new DataStoreException("Invalid reference");
                        }
                        else if (retVal != DATASTORE_RETURN_STATUS.Success)
                            Debug.Print("Read failed");
                        else
                            Debug.Print("Read succeeded");
                    }
                }
                catch (Exception ex)
                {
                    Debug.Print("Read failed. Exception is: " + ex.Message);
                }

                ++index;
            }

            //Delete a random reference
            randValue = rand.Next(experimentIndex);
            if (dataRefArray[randValue].Delete() != DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Data delete failed");
            else
                Debug.Print("Data delete succeeded");

            //Read from same reference
            try
            {
                retVal = dataRefArray[randValue].Read(readBufferByte, offset, readBufferByte.Length);

                if (retVal == DATASTORE_RETURN_STATUS.InvalidReference)
                {
                    Debug.Print("Read failed. Invalid reference");
                    throw new DataStoreException("Invalid reference");
                }
                else if (retVal != DATASTORE_RETURN_STATUS.Success)
                    Debug.Print("Read failed");
                else
                    Debug.Print("Read succeeded");
            }
            catch (Exception ex)
            {
                Debug.Print("Read failed. Exception is: " + ex.Message);
            }
        }


        public static void Main()
        {
            CompleteAppNote appNote = new CompleteAppNote();
            appNote.Program();
        }

    }
}
