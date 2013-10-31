using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using System.Threading;

namespace Samraksh.SPOT.Tests
{
    public class NORTest
    {
        public const UInt16 BufferSize = 768;
        public static Random rng = new Random();
        public static UInt16[] buffer1 = new UInt16[BufferSize];
        public static UInt16 norSize = 0;
        public static UInt16 norReadBytes = 0;

        public NORTest()
        {
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Initialize(8 * 1024);
        }

        public bool Run()
        {

            string writtenData = "";

            while (true)
            {
                writtenData = "Data";

                norSize += BufferSize;

                Debug.Print("Working...\n");

                for (UInt16 i = 0; i < BufferSize; i++)
                {
                    buffer1[i] = (UInt16)rng.Next(1 << 12);
                    writtenData += buffer1[i].ToString() + ",";
                }

                Debug.Print(writtenData.ToString());

                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.IsFull())
                {
                    Debug.Print("NOR is full\n");
                    return false;
                }

                if (!Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(buffer1, BufferSize))
                {
                    Debug.Print("Failed to Write To NOR\n");
                    return false;
                }

                Thread.Sleep(200);

            }
            
        }

        public void Exfiltrate()
        {

            string readData = "";

            while (!Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof())
            {

                readData = "Data";

                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(buffer1, BufferSize) != DeviceStatus.Success)
                {
                    Debug.Print("Failed to read from NOR\n");
                    return;
                }

                for (UInt16 i = 0; i < BufferSize; i++)
                {
                    readData += buffer1[i].ToString() + ",";
                }

                Debug.Print(readData.ToString());

                readData = "";

                norReadBytes += BufferSize;

                Thread.Sleep(200);
            }

        }

        public static void Main()
        {

            Debug.EnableGCMessages(false);

            NORTest norTest = new NORTest();

            norTest.Run();

            Debug.Print("Wrote " + (norSize * 2).ToString() + " bytes\n");

            norTest.Exfiltrate();

            Debug.Print("Read " + (norReadBytes * 2).ToString() + " bytes\n");

            

        }

    }
}
