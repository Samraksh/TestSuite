using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Mac;
using Samraksh.SPOT.Net;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace BasicPing
{

    public class PingMsg
    {
        public bool Response;
        public ushort MsgID;
        public UInt16 Src;
        public UInt16 dummySrc;

        public PingMsg()
        {
        }
        public static int Size()
        {
            return 7;
        }
        public PingMsg(byte[] rcv_msg, ushort size)
        {
            Response = rcv_msg[0] == 0 ? false : true;
            MsgID = (UInt16)(rcv_msg[1] << 8);
            MsgID += (UInt16)rcv_msg[2];
            Src = (UInt16)(rcv_msg[3] << 8);
            Src += (UInt16)rcv_msg[4];
            dummySrc = (UInt16)(0xefef);

        }

        public byte[] ToBytes()
        {
            byte[] b_msg = new byte[7];
            b_msg[0] = Response ? (byte)1 : (byte)0;
            b_msg[1] = (byte)((MsgID >> 8) & 0xFF);
            b_msg[2] = (byte)(MsgID & 0xFF);
            b_msg[3] = (byte)((Src >> 8) & 0xFF);
            b_msg[4] = (byte)(Src & 0xFF);
            b_msg[5] = (byte)(0xef);
            b_msg[6] = (byte)(0xef);
            return b_msg;
        }
    }

    

    public class MasterBasicPing
    {
        CSMA myCSMA;

        static OutputPort SendPort = new OutputPort((Cpu.Pin)30, true);
        static OutputPort ReceivePort = new OutputPort((Cpu.Pin)31, true);

        Message recvMessage;

        MacConfiguration csmaConfig = null;

        ReceiveCallBack callback;

        PingMsg sendMsg = new PingMsg();

        UInt16 mySeqNo = 0;

        UInt16 myAddress;

        NeighbourhoodChangeCallBack myNeighborCB;

        Timer sendTimer;

        void HandleMessage(UInt16 numberOfPackets)
        {
            recvMessage = myCSMA.GetNextPacket();

            
        }

        void NeighborChange(UInt16 noOfNeigbors)
        {
        }

        public MasterBasicPing()
        {
            csmaConfig = new MacConfiguration();

            callback = HandleMessage;

            myNeighborCB = NeighborChange;

            
            // Configure the CSMA object before creating the instance
            if (CSMA.Configure(csmaConfig, callback, myNeighborCB) != DeviceStatus.Success)
            {
                Debug.Print("The CSMA configure call failed\n");
            }

            try
            {
                // Grab the csma instance 
                myCSMA = CSMA.Instance;
            }
            catch (MacNotConfiguredException m)
            {
                Debug.Print("Exception in MasterBasicPing due to Mac not being configured\n");
            }
            catch (Exception e)
            {
                Debug.Print("Unknown exception in MasterBasicPing\n");
            }

            myAddress = myCSMA.GetAddress();

            //myCSMA.SetAddress(52);

            Debug.Print("CSMA Init done. \n");
            
        }

        public void Run()
        {
            Debug.Print("Starting timer...\n");
            sendTimer = new Timer(Sender, null, 0, 200);
            Debug.Print("Timer init done.");
        }

        void Send_Ping(PingMsg ping)
        {
            //UInt16 sender = ping.Src;
            //Debug.GC(true);
            Debug.Print("Sending Broadcast message : " + mySeqNo.ToString() + " from " + myAddress.ToString()); 

            ping.Response = false;
            ping.MsgID = mySeqNo++;
            ping.Src = myAddress;

            SendPort.Write(true);
            SendPort.Write(false);

            byte[] msg = ping.ToBytes();
            myCSMA.Send((UInt16)Addresses.BROADCAST, msg, 0, (ushort)msg.Length);
            
            

        }

        void Sender(Object state)
        {
            Send_Ping(sendMsg);
        }


        public static void Main()
        {
            MasterBasicPing prg = new MasterBasicPing();

            prg.Run();

            while (true)
            {
                Thread.Sleep(1000);
            }
        }

    }
}
