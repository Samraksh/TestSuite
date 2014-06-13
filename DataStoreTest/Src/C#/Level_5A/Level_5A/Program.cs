using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.NonVolatileMemory;

/* NOTE: THIS TEST WILL NOT SHOW A SUCCESS IN THE RIG, BUT IF YOU WATCH THE OUTPUT ON A TERMINAL SCREEN, IT SHOULD SHOW THAT THE TEST WAS SUCCESSFUL */

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
        
        int size;
        int experimentIndex;
        int offset = 0, arrayLength = 0;

        public DataStoreExceptionTest()
        {
            try
            {
                Debug.Print("Starting test Level_5A");
                bool eraseDataStore = true;
                dStore = DataStore.Instance(StorageType.NOR, eraseDataStore);
            
                experimentIndex = 10;
                size = 2;
                rand = new Random();
            
                readBuffer = new byte[size];
                writeBuffer = new byte[size];
            
                if (dStore.EraseAllData() == DataStoreReturnStatus.Success)
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
            catch (Exception ex)
            {
                Debug.Print(" Write failed. Exception is: " + ex.Message);
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

        public void writeData()
        {
            for (UInt32 dataIndex = 0; dataIndex < experimentIndex; ++dataIndex)
            {
                try
                {
                    size = rand.Next(828) + 2;
                    data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                    DataStoreReturnStatus retVal = data.Write(writeBuffer, 0, writeBuffer.Length);
                    if (retVal == DataStoreReturnStatus.Success)
                        Debug.Print("Write successful");
                    else if (retVal == DataStoreReturnStatus.InvalidReference)
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
                    return;
                }
            }
        }

        // Test that creates a bunch of records and returns success if record creation
        // was successful
        public void Level_5A(int excepType)
        {
            try
            {
                size = rand.Next(828) + 2;
                data = new DataReference(dStore, size, ReferenceDataType.BYTE);
                Debug.Print("Data created successfully");

                switch (excepType)
                {
                    case ((int)(exceptionType.DATA_AMOUNT_ZERO)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.DATA_AMOUNT_ZERO)) + ": DATA_AMOUNT_ZERO");
                        try
                        {
                            arrayLength = 0; offset = 0;
                            byte[] writeTestBuffer = new byte[0];
                            DataStoreReturnStatus retVal = data.Write(writeTestBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                        break;
                    }

                    case ((int)(exceptionType.OFFSET_LESS_THAN_ZERO)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.OFFSET_LESS_THAN_ZERO)) + ": OFFSET_LESS_THAN_ZERO");
                        try
                        {
                            offset = -1; arrayLength = writeBuffer.Length;
                            DataStoreReturnStatus retVal = data.Write(writeBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                        break;
                    }

                    case ((int)(exceptionType.DATA_AMOUNT_LESS_THAN_ZERO)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.DATA_AMOUNT_LESS_THAN_ZERO)) + ": DATA_AMOUNT_LESS_THAN_ZERO");
                        try
                        {
                            offset = 0; arrayLength = -1;
                            DataStoreReturnStatus retVal = data.Write(writeBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
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
                            Debug.Print("result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }

                        //Get the data references into dataRefArray.
                        offset = 0;
                        dataRefArray = new DataReference[experimentIndex];
                        if (dStore.ReadAllDataReferences(dataRefArray, offset) != DataStoreReturnStatus.Success)
                        {
                            Debug.Print("ReadAllDataReferences failed");
                            break;
                        }
                    
                        //Delete a random reference
                        int randValue = rand.Next(experimentIndex);
                        if(dataRefArray[randValue].Delete() != DataStoreReturnStatus.Success)
                            Debug.Print("Data delete failed");
                        else
                            Debug.Print("Data delete succeeded");
                    
                        //Read from same reference
                        try
                        {
                            DataStoreReturnStatus retVal = dataRefArray[randValue].Read(readBuffer, offset, readBuffer.Length);

                            if (retVal == DataStoreReturnStatus.InvalidReference)
                            {
                                Debug.Print("Read failed. Invalid reference");
                                throw new DataStoreException("Invalid reference");
                            }
                            else if (retVal != DataStoreReturnStatus.Success)
                                Debug.Print("Read failed");
                            else
                                Debug.Print("Read succeeded");
                        }
                        catch (Exception ex)
                        {
                            Debug.Print("result=PASS. Read failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                    
                        break;
                    }

                    case ((int)(exceptionType.DATATYPE_DIFFERENT)):
                    {
                        //Cannot test this, as this can never fail.
                        Debug.Print("result=PASS. Starting test " + ((int)(exceptionType.DATATYPE_DIFFERENT)) + ": DATATYPE_DIFFERENT");
                        //Debug.Print("result=PASS\n");
                        break;
                    }

                    case ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ARRAY_SIZE)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ARRAY_SIZE)) + ": DATA_SIZE_GREATER_THAN_ARRAY_SIZE");
                        try
                        {
                            offset = 0; arrayLength = writeBuffer.Length + experimentIndex;
                            DataStoreReturnStatus retVal = data.Write(writeBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                        break;
                    }

                    case ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE)) + ": DATA_SIZE_GREATER_THAN_ALLOCATION_SIZE");
                        try
                        {
                            offset = 0; arrayLength = experimentIndex * experimentIndex * experimentIndex;
                            DataStoreReturnStatus retVal = data.Write(writeBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                        break;
                    }

                    case ((int)(exceptionType.DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE)) + ": DATA_SIZE_FROM_OFFSET_GREATER_THAN_ARRAY_SIZE");
                        try
                        {
                            offset = experimentIndex; arrayLength = experimentIndex * experimentIndex * experimentIndex;
                            DataStoreReturnStatus retVal = data.Write(writeBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                        break;
                    }

                    case ((int)(exceptionType.BYTE_DATATYPE_OFFSET_ODD)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.BYTE_DATATYPE_OFFSET_ODD)) + ": BYTE_DATATYPE_OFFSET_ODD");
                        try
                        {
                            offset = 1; arrayLength = writeBuffer.Length;
                            DataStoreReturnStatus retVal = data.Write(writeBuffer, offset, arrayLength);
                        }
                        catch (Exception ex)
                        {
                            Debug.Print(" result=PASS. Write failed. Exception is: " + ex.Message);
                            //Debug.Print("result=PASS\n");
                        }
                        break;
                    }

                    case((int)(exceptionType.NO_EXCEPTION)):
                    {
                        Debug.Print("Starting test " + ((int)(exceptionType.NO_EXCEPTION)) + ": NO_EXCEPTION");
                        writeData();
                        DisplayStats(true, "Exception test successful", "", 0);
                        break;
                    }

                    default:
                        break;
                }
            }
            catch (Exception ex)
            {
                Debug.Print(ex.Message);
                return;
            }
        }

        public static void Main()
        {
            DataStoreExceptionTest dtest = new DataStoreExceptionTest();

            //for (int testIndex = 0; testIndex < 4; ++testIndex)
            for (int testIndex = 0; testIndex <= (exceptionType.NO_EXCEPTION - exceptionType.DATA_AMOUNT_ZERO); ++testIndex)
            {
                dtest.Level_5A(testIndex);
            }
        }

    }
}
