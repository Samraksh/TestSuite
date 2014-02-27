using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

using _DBG = Microsoft.SPOT.Debugger;
using _WP = Microsoft.SPOT.Debugger.WireProtocol;

namespace SerialTest
{
    public class SerialTester
    {
        Stream UsartStream;
        AsyncCallback serialCallback;
        static byte[] buffer = new byte[255];
        static bool dDone = true;
        static byte[] cacheBuffer = new byte[255];
        bool packetFound = false;
        int packetLost = 0;
        int packetRecieved = 0;
        static byte[] sendBuffer = new byte[255];
        int transmittedPackets = 0;
        int recvdPackets = 0;
        double accuracy = 0.0;
        Thread m_sendThread;
        StreamWriter inpFile;
        StreamWriter OutFile;

        public SerialTester()
        {
            _DBG.PortDefinition pd = null;
            string port = "COM165";
            uint baudrate = 115200;

            pd = _DBG.PortDefinition.CreateInstanceForSerial(port, port, baudrate);

            if (pd.TryToOpen())
            {
                UsartStream = pd.Open();

                Console.WriteLine("Opening Port : " + pd.DisplayName.ToString());

            

            }
            else
            {
                Console.WriteLine("The port can not be openend");
                throw new UnableToOpenPortException();
            }

            //inpFile = new StreamWriter("Input.txt", true);
            //OutFile = new StreamWriter("Output.txt", true);

        }

        public void Run_MasterMode()
        {
           serialCallback = new AsyncCallback(ProcessData_MasterMode);

           m_sendThread = new Thread(new ThreadStart(SendThread));
           m_sendThread.Start();

           int count = 0;
           Int32 Size = 0;

           while (true)
           {
               if (dDone)
               {
                   dDone = false;

                   UsartStream.BeginRead(buffer, 0, buffer.Length, serialCallback, count);
                   ++count;

               }
           }
           
        }


        public void SendThread()
        {
            UInt32 i = 0;

            while (i++ < 100)
            {

                System.Random prng = new System.Random();


                int Size = 0;

                Size = prng.Next(4,253);

                prng.NextBytes(sendBuffer);

                for (int k = 0; k < sendBuffer.Length; k++)
                {
                    if (sendBuffer[k] == 255)
                    {
                        sendBuffer[k] = 7;
                    }
                }

                sendBuffer[0] = 254;

                sendBuffer[1] = (byte)Size;

                sendBuffer[Size - 1] = 255;            

                cacheBuffer = sendBuffer;

                for (int j = 0; j < Size; j++)
                {
                    inpFile.Write(cacheBuffer[j].ToString());   
                }

                inpFile.WriteLine();

                transmittedPackets++;

                Console.WriteLine("Transmitting Packet " + i.ToString());

                UsartStream.Write(sendBuffer, 0, Size);



                Thread.Sleep(300);

            }

            accuracy = recvdPackets / transmittedPackets;

            Console.WriteLine("Number of packets transmitted " + transmittedPackets + "\n");
            Console.WriteLine("Number of packets received " + recvdPackets + "\n");
            Console.WriteLine("Accuracy = " + accuracy.ToString() + "\n");

           
        }

        public void Run_SlaveMode()
        {
            serialCallback = new AsyncCallback(ProcessData);
            int count = 0;
            int Size = 0;

            while (true)
            {
                if (dDone)
                {
                    dDone = false;

                   
                    UsartStream.BeginRead(buffer, 0, buffer.Length, serialCallback, count++);
                    
                }

            }
        }

        public void SendOverSerial(int packetLength)
        {
            try
            {
                UsartStream.Write(buffer, 0, packetLength);
            }
            catch (IOException e)
            {
                Console.WriteLine("The Write Operation could not be performed");
            }
        }

        public bool Verify()
        {
            for (int i = 0; i < sendBuffer[0]; i++)
            {
                if (buffer[i] != sendBuffer[i])
                    return false;
            }

            return true;
        }

        public void ProcessData_MasterMode(IAsyncResult result)
        {
           
           for (int i = 0; i < buffer.Length; i++)
           {
               OutFile.Write(buffer[i].ToString());

               if (buffer[i] == 255)
                   OutFile.WriteLine();
                
           }
           //OutFile.WriteLine("\n");
           
           dDone = true;

        }

        public void ProcessData(IAsyncResult result)
        {
            int i = 0;

            packetRecieved++;

            for (i = 0; i < buffer[0]; i++)
            {
                // Send back data
                if (buffer[i] == 254)
                {
                    packetFound = true;
                    break;
                }
               
            }
            if (packetFound)
                SendOverSerial(i);
            else
            {
                packetLost++;
                Console.WriteLine("Incomplete packet recieved ");
            }

            dDone = true;

        }

    }

    public class Program
    {
        static void Main(string[] args)
        {
            SerialTester serialTesterObject = new SerialTester();

            //serialTesterObject.Run_MasterMode();
            serialTesterObject.Run_SlaveMode();
        }
    }
}
