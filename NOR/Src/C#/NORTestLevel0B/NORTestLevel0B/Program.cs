using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{
    public abstract class PersistentStorage
    {
        abstract public bool Write(ushort[] data, UInt16 length);

        abstract public bool Write(byte[] data, UInt16 length);

        abstract public byte[] ReadByte(UInt16 length);

        abstract public ushort[] ReadUshort(UInt16 length);

        abstract public bool WriteEof();

        abstract public bool eof();

        abstract public bool IsFull();

    }

    public class NorStore : PersistentStorage
    {
        public const uint NorSize = 2 * 512 * 1024;

        public NorStore()
        {
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Initialize(NorSize);
        }


        public override bool IsFull()
        {
            return Samraksh.SPOT.Hardware.EmoteDotNow.NOR.IsFull();
        }

        public override bool Write(byte[] data, UInt16 length)
        {
            ushort[] tempdata = new ushort[length/2];
            uint dataCounter = 0;
            uint i = 0;

            while (i < length)
            {
                tempdata[dataCounter] = (ushort)(((ushort)data[i++]) << 8);
                tempdata[dataCounter] |= (ushort)((ushort)data[i++]);
                dataCounter++;
            }

            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(tempdata, (ushort) (length/2));

            return true;

        }

        public override bool Write(ushort[] data, UInt16 length)
        {
            if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.IsFull())
            {
                return false;
            }

            // Writing to NOR flash
            //if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.StartNewRecord())
            //{
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(data, length);

            //    Samraksh.SPOT.Hardware.EmoteDotNow.NOR.EndRecord();
            //}

            return true;

        }

        public override bool eof()
        {
            return Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof();
        }


        public override byte[] ReadByte(UInt16 length)
        {
            ushort[] tempdata = new ushort[length / 2];
            uint dataCounter = 0;

            byte[] data = new byte[length];

            //if (!Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof())
            //{
                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(tempdata, (ushort) (length / 2)) != Samraksh.SPOT.Hardware.DeviceStatus.Success)
                {
                    Debug.Print("Read from NOR failed \n");
                    return null;
                }
            //}
            //else
            //    return null;

            

            for (uint i = 0; i < length / 2; i++)
            {
                //Debug.Print(tempdata[i].ToString());
                data[dataCounter++] = (byte) (tempdata[i] >> 8);
                data[dataCounter++] = (byte)(tempdata[i]);
            }

            return data;

        }

        public override ushort[] ReadUshort(UInt16 length)
        {

            ushort[] tempData = new ushort[length];

            if (!Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof())
            {
                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(tempData, length) != Samraksh.SPOT.Hardware.DeviceStatus.Success)
                {
                    Debug.Print("Read from NOR failed \n");
                    return null;
                }
            }
            else
                return null;

            return tempData;
        }

        public override bool WriteEof()
        {
            ushort[] eof = new ushort[512];

            for (UInt16 i = 0; i < eof.Length; i++)
            {
                eof[i] = 0x0c0c;
            }

            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(eof, (ushort)eof.Length);

            return true;
        }
    }

    public class NORTestLevel0B
    {

        public PersistentStorage norStorage;
        public Random rnd;
        public uint errorCounter;

        public uint bytesWritten;
        
        public const int BufferSize = 1024;

        public byte[] wdata = new byte[BufferSize];
        public byte[] rdata = new byte[BufferSize];

        public ushort[] wdataushort = new ushort[BufferSize];
        public ushort[] rdataushort = new ushort[BufferSize];

        bool readDone = false;

        public NORTestLevel0B()
        {

            Debug.Print("Initializing NOR ...");
            norStorage = new NorStore();

            rnd = new Random();

            errorCounter = 0;
            bytesWritten = 0;

        }

        public void Flush()
        {
            for (uint i = 0; i < BufferSize; i++)
            {
                //rdata[i] = 0;
                wdata[i] = 0;
            }
        }


        public void RunControlled()
        {
            ushort i = 0;
            uint arrayIndex = 0;

            uint bytesWritten = 0;

            while (!norStorage.IsFull())
            {
                Flush();

                for (arrayIndex = 0; arrayIndex < wdataushort.Length;)
                {
                    if (i == 65000)
                        i = 0;

                    wdataushort[arrayIndex++] = i;

                    i++;
                }

                norStorage.Write(wdataushort, (ushort) wdataushort.Length);
                bytesWritten += (uint) (wdataushort.Length * 2);
            }

            Debug.Print("The number of bytes written " + bytesWritten.ToString());
            Debug.Print("The current value of i is " + i.ToString());

            norStorage.WriteEof();

            // Check what was written
            while (true)
            {
                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(rdataushort, (ushort)rdataushort.Length) != DeviceStatus.Success)
                {
                    Debug.Print("Failed to read from NOR\n");
                    
                }

                for (UInt16 j = 0; j < (rdataushort.Length - 1); j++)
                {
                    if (rdataushort[j + 1] - rdataushort[j] != 1 && rdataushort[j] != 3084)
                    {
                        Debug.Print("Anomaly Detected at " + j.ToString() + " and " + (j + 1).ToString() + " values are " + rdataushort[j].ToString() + "\t" + rdataushort[j + 1].ToString());
                    }
                }

                if (rdataushort[0] == 0x0c0c && rdataushort[1] == 0x0c0c && rdataushort[2] == 0x0c0c && rdataushort[3] == 0x0c0c)
                {
                    readDone = true;
                    break;
                }

                if (readDone)
                {
                    //Debug.Print("Read Done ...");
                    break;
                }
            }

            Debug.Print("Test Complete");

        }

        public void RunRandomized()
        {
            

            while (!norStorage.IsFull())
            {
                Flush();

                rnd.NextBytes(wdata);

                norStorage.Write(wdata, BufferSize);

                byte[] rdata = norStorage.ReadByte(BufferSize);

                for (uint i = 0; i < BufferSize; i++)
                {
                    if (wdata[i] != rdata[i])
                    {
                        //Debug.Print(wdata[i].ToString() + "\t" + rdata[i].ToString());
                        errorCounter++;
                    }
                }

                bytesWritten += BufferSize;

                Debug.Print("Bytes Written : " + bytesWritten.ToString() + "\n");
                Debug.Print("Holes : " + errorCounter.ToString() + "\n");
                errorCounter = 0;

            }
        }

        public static void Main()
        {
            Debug.EnableGCMessages(false);

            NORTestLevel0B test = new NORTestLevel0B();

            test.RunControlled();
        }

    }
}
