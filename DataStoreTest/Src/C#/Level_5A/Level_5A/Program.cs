using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.NonVolatileMemory;

/* This test is to verify all exception conditions during write 
 * 1. Array length is zero. Exception expected                                                  -- DataStoreException("data array cannot be of zero length");  
 * 2. Offset less than zero. Exception expected                                                 -- DataStoreException("Offset should not be negative");
 * 3. Amount of data to be written less than zero. Exception expected                           -- DataStoreException("numData should not be negative");
 * 4. Null reference. Exception expected                                                        -- DataStoreException("Reference to data is null");
 * 5. DataType different from the one during creation. Exception expected                       -- DataStoreException("DataType is not the same as the one during creation");
 * 6. Size of data to be written greater than array size. Exception expected                    -- DataStoreException("Amount of data to be written is greater than array size");
 * 7. Size of data to be written greater than allocation size. Exception expected               -- DataStoreException("Amount of data to be written is greater than size of data allocated");
 * 8. Size of data to be written from offset is greater than allocation size. Exception expected-- DataStoreException("Amount of data to be written exceeds size of data allocated");
 * 9. Only for byte dataType                                                                    -- DataStoreException("Offset should not be odd for byte dataType");*/

namespace Level_5A
{

    public enum exceptionType
    {
        DATA_AMOUNT_ZERO = 0,
        OFFSET_LESS_THAN_ZERO = 1,
        DATA_AMOUNT_LESS_THAN_ZERO = 2,
        NULL_REFERENCE = 3,
        DATATYPE_DIFFERENT = 4,
        DATA_SIZE_GREATER_THAN_ARRAY_SIZE = 5,
        DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE = 6,
        DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE = 7,
        BYTE_DATATYPE_OFFSET_ODD = 8,
        NO_EXCEPTION = 9
    }

    public class DataStoreExceptionTest
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
        int offset = 0, arrayLength = 0;

        public DataStoreExceptionTest()
        {
            Debug.Print("Starting test Level_5A");
            dStore = DataStore.Instance(StorageType.NOR);
            
            experimentIndex = 10;
            size = 2;
            rand = new Random();
            
            readBuffer = new byte[size];
            writeBuffer = new byte[size];
            dataType = typeof(byte);

            if (DataStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");
            else
            {
                Debug.Print("Datastore could not be erased");
            }

            for (UInt16 writeIndex = 0; writeIndex < writeBuffer.Length; ++writeIndex)
            {
                writeBuffer[writeIndex] = (byte)writeIndex;
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
                    data = new DataReference(dStore, size, dataType);
                    DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, 0, writeBuffer.Length);
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

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_5A(int excepType)
        {
            size = rand.Next(828) + 2;
            data = new DataReference(dStore, size, dataType);

            switch (excepType)
            {
                case ((int)(exceptionType.DATA_AMOUNT_ZERO)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.DATA_AMOUNT_ZERO)) + ": DATA_AMOUNT_ZERO");
                    try
                    {
                        arrayLength = 0; offset = 0;
                        byte[] writeTestBuffer = new byte[0];
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeTestBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case ((int)(exceptionType.OFFSET_LESS_THAN_ZERO)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.OFFSET_LESS_THAN_ZERO)) + ": OFFSET_LESS_THAN_ZERO");
                    try
                    {
                        offset = -1; arrayLength = writeBuffer.Length;
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case ((int)(exceptionType.DATA_AMOUNT_LESS_THAN_ZERO)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.DATA_AMOUNT_LESS_THAN_ZERO)) + ": DATA_AMOUNT_LESS_THAN_ZERO");
                    try
                    {
                        offset = 0; arrayLength = -1;
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case ((int)(exceptionType.NULL_REFERENCE)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.NULL_REFERENCE)) + ": NULL_REFERENCE");
                    //Write data
                    try
                    {
                        writeData();
                    }
                    catch (Exception ex)
                    {
                        Debug.Print("Write failed. Exception is: " + ex.Message);
                    }

                    //Get the data references into dataRefArray.
                    offset = 0;
                    dataRefArray = new DataReference[experimentIndex];
                    if (dStore.ReadAllDataReferences(dataRefArray, offset) != DATASTORE_RETURN_STATUS.Success)
                    {
                        Debug.Print("ReadAllDataReferences failed");
                        break;
                    }
                    
                    //Delete a random reference
                    int randValue = rand.Next(experimentIndex);
                    if(dataRefArray[randValue].Delete() != DATASTORE_RETURN_STATUS.Success)
                        Debug.Print("Data delete failed");
                    else
                        Debug.Print("Data delete succeeded");
                    
                    //Read from same reference
                    try
                    {
                        DATASTORE_RETURN_STATUS retVal = dataRefArray[randValue].Read(readBuffer, offset, readBuffer.Length);

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
                    
                    break;
                }

                case ((int)(exceptionType.DATATYPE_DIFFERENT)):
                {
                    //Cannot test this, as this can never fail.
                    Debug.Print("Starting test " + ((int)(exceptionType.DATATYPE_DIFFERENT)) + ": DATATYPE_DIFFERENT");
                    break;
                }

                case ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ARRAY_SIZE)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ARRAY_SIZE)) + ": DATA_SIZE_GREATER_THAN_ARRAY_SIZE");
                    try
                    {
                        offset = 0; arrayLength = writeBuffer.Length + experimentIndex;
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE)) + ": DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE");
                    try
                    {
                        offset = 0; arrayLength = experimentIndex * experimentIndex * experimentIndex;
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case ((int)(exceptionType.DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE)) + ": DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE");
                    try
                    {
                        offset = experimentIndex; arrayLength = experimentIndex * experimentIndex * experimentIndex;
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case ((int)(exceptionType.BYTE_DATATYPE_OFFSET_ODD)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.BYTE_DATATYPE_OFFSET_ODD)) + ": BYTE_DATATYPE_OFFSET_ODD");
                    try
                    {
                        offset = 1; arrayLength = writeBuffer.Length;
                        DATASTORE_RETURN_STATUS retVal = data.Write(writeBuffer, offset, arrayLength);
                    }
                    catch (Exception ex)
                    {
                        Debug.Print(" Write failed. Exception is: " + ex.Message);
                    }
                    break;
                }

                case((int)(exceptionType.NO_EXCEPTION)):
                {
                    Debug.Print("Starting test " + ((int)(exceptionType.NO_EXCEPTION)) + ": NO_EXCEPTION");
                    writeData();
                    break;
                }

                default:
                    break;
            }

        }

        public static void Main()
        {
            DataStoreExceptionTest dtest = new DataStoreExceptionTest();

            for (int testIndex = 0; testIndex <= (exceptionType.NO_EXCEPTION - exceptionType.DATA_AMOUNT_ZERO); ++testIndex)
            //for (int testIndex = 0; testIndex < 4; ++testIndex)
            {
                dtest.Level_5A(testIndex);
            }
        }

    }
}
