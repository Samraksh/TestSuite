using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.IO.Ports;

namespace USARTTest
{

    public class Serial
    {
        public SerialPort serialPortInstance;
        public byte[] recvBuffer;
        public byte[] store;
        public int bytesRead;
        public int dataToRead;
        public bool dataReady;
        public int storeCount = 0;
        
        

        public Serial()
        {
            bytesRead = 0;
            dataToRead = 0;
            recvBuffer = new byte[255];
            store = new byte[255];
            dataReady = false;

            serialPortInstance = new SerialPort("COM1", 115200);
            serialPortInstance.DataReceived += ReceiveHandler;
            serialPortInstance.Open();
            
        }

        public bool IsRecvBufferEmpty()
        {
            if (bytesRead == 0)
                return true;
            else
                return false;
        }

        public void ReceiveHandler(Object sender, SerialDataReceivedEventArgs e)
        {

            if (serialPortInstance.IsOpen)
            {
                if (serialPortInstance.BytesToRead > 0)
                {
                    int bytesToRead = serialPortInstance.BytesToRead;

                    serialPortInstance.Read(recvBuffer, 0, bytesToRead);

                    for (int i = 0; i < bytesToRead; i++)
                    {
                        if (recvBuffer[i] == 255)
                        {
                            store[storeCount++] = recvBuffer[i];  
                            dataToRead = storeCount;
                            storeCount = 0;
                            dataReady = true;
                            break;
                        }
                        store[storeCount++] = recvBuffer[i];  
                    }

                    /*
                    if (storeCount == dataToRead)
                    {
                        storeCount = 0;
                        dataReady = true;
                    }
                     */
                    
                }
            }
        }

        public bool DataArrived()
        {
            return dataReady;
        }

        public int ReadByte()
        {
            return serialPortInstance.ReadByte();
        }

        public byte[] GetBuffer()
        {
            return store;
        }

        public void Send(byte[] data, int count)
        {
            if (serialPortInstance.IsOpen)
            {
                serialPortInstance.Write(data, 0, count);
                serialPortInstance.Flush();
            }
            else
            {
                Debug.Print("The Serial Port Instance is not open");
            }
        }
    }

    public class Program
    {

        public Random rnd;
        public Serial srl;
        public byte[] data;
        public int transmittedPackets;
        public int verifiedPackets;
        public int packetSize;

        public Program()
        {
            rnd = new Random();
            srl = new Serial();
            data = new byte[255];
            transmittedPackets = 0;
            verifiedPackets = 0;
            packetSize = 0;
        }

        public void LoadBuffer()
        {
            packetSize = rnd.Next(253);

            rnd.NextBytes(data);

            data[0] = 255;
            data[1] = (byte)packetSize;
            data[packetSize - 1] = 254;

        }

        public void TransmitData()
        {
            transmittedPackets++;
            LoadBuffer();
            srl.dataToRead = data[1];
            srl.Send(data, packetSize);
        }

        public bool Verify(byte[] recvData)
        {
            UInt32 i = 0;

            while (i++ != data[1])
            {
                if (data[i] != recvData[i])
                    return false;
            }
            return true;
        }

        public void DisplayStats(string result, string resultParameter1, string resultParameter2, string accuracy)
        {
            Debug.Print("result =  " + result + "\n");
            Debug.Print("resultParameter1 =  " + resultParameter1 + "\n");
            Debug.Print("resultParameter2 =  " + resultParameter2 + "\n");
            Debug.Print("accuracy =  " + accuracy + "\n");

        }

        public void MasterMode()
        {
            UInt32 i = 0;

            UInt32 numberOfIterations = 200;

            while (i++ < numberOfIterations)
            {

                TransmitData();

                //Thread.Sleep(200);


                while (!srl.DataArrived()) ;

                byte[] recvData = srl.GetBuffer();

                if (Verify(recvData))
                {
                    verifiedPackets++;
                }
            }

            double accuracy = verifiedPackets / transmittedPackets;
            if (accuracy < 0.5)
            {
                  DisplayStats("false", "", "", accuracy.ToString());
            }
            else
            {
                 DisplayStats("true", "", "", accuracy.ToString());
            }
                
        }

        public void SlaveMode()
        {

            Debug.EnableGCMessages(false);

            while (true)
            {

                while (!srl.DataArrived()) ;

                byte[] recvData = srl.GetBuffer();

                srl.Send(recvData, srl.dataToRead);

                srl.dataReady = false;

                srl.dataToRead = 0;
                srl.bytesRead = 0;

                Thread.Sleep(100);

            }


        }

        public static void Main()
        {

            Debug.Print("Running Application\n");

            Program serialTestPrg = new Program();


            serialTestPrg.SlaveMode();
            
            
            

        }

    }
}
