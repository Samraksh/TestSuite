using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Mac;
using Samraksh.SPOT.Net;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace ChannelChangeTest
{

    public class State
    {
        private readonly Object _lock = new Object();
        private bool ready;

        public State()
        {
            ready = false;
        }

        public bool IsReady()
        {
            lock (_lock)
            {
                return ready;
            }
        }

        public void SetState(bool state)
        {
            lock (_lock)
            {
                ready = state;
            }
        }

    }

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

    public class Slave
    {
        CSMA myCSMA = new CSMA();

        UInt16 myAddress;

        UInt16 mySeqNo = 0;

        PingMsg sendMsg = new PingMsg();

        Samraksh.SPOT.Net.Radio.Channels currChannel;

        State recvd;

        MacConfiguration macConfig = new MacConfiguration();
        ReceiveCallBack myReceive;

        static OutputPort SendPort = new OutputPort((Cpu.Pin)30, true);
        static OutputPort ReceivePort = new OutputPort((Cpu.Pin)31, true);

        Random rnd = new Random(15);

        Timer sendTimer;

        Timer channelChangeTimer;

        public void Initialize()
        {
            Debug.Print("Initializing Change Channel Test");

            recvd = new State();

            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            //macConfig.RadioID = (byte) myRadioID;
            macConfig.RadioID = (byte)1;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            myReceive = HandleMessage; //Assign the delegate to a function

            Debug.Print("Initializing:  CSMA...");
            try
            {
                myCSMA.Initialize(macConfig, myReceive);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
            Debug.Print("CSMA Init done.");
            myAddress = myCSMA.GetAddress();
            Debug.Print("My default address is :  " + myAddress.ToString());

            currChannel = Samraksh.SPOT.Net.Radio.Channels.Channel_26;

        }

        string PrintChannel(Samraksh.SPOT.Net.Radio.Channels channel)
        {
            switch (channel)
            {
                case Samraksh.SPOT.Net.Radio.Channels.Channel_11:
                    return "11";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_12:
                    return "12";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_13:
                    return "13";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_14:
                    return "14";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_15:
                    return "15";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_16:
                    return "16";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_17:
                    return "17";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_18:
                    return "18";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_19:
                    return "19";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_20:
                    return "20";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_21:
                    return "21";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_22:
                    return "22";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_23:
                    return "23";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_24:
                    return "24";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_25:
                    return "25";
                case Samraksh.SPOT.Net.Radio.Channels.Channel_26:
                    return "26";
                default:
                    return "0";
            }
        }

        public void Start()
        {
            Debug.Print("Starting timer...");
            //sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, 100);
            channelChangeTimer = new Timer(new TimerCallback(channelChange), null, 0, 10000);
            Debug.Print("Timer init done.");
        }

        void channelChange(Object o)
        {

            if (currChannel == (Samraksh.SPOT.Net.Radio.Channels.Channel_26 + 1))
            {
                currChannel = Samraksh.SPOT.Net.Radio.Channels.Channel_11;
            }

            Debug.Print("Scanning Channel " + PrintChannel(currChannel));

            myCSMA.SetChannel((int)currChannel++);
        }

        

        void Send_Ping(PingMsg ping)
        {
            //UInt16 sender = ping.Src;
            //Debug.GC(true);
            ping.Response = false;
            ping.MsgID = mySeqNo++;
            ping.Src = myAddress;

            SendPort.Write(true);
            SendPort.Write(false);

            byte[] msg = ping.ToBytes();
            myCSMA.Send((UInt16)Addresses.BROADCAST, msg, 0, (ushort)msg.Length);
            //int char0 = (mySeqNo % 10) + (int)LCD.CHAR_0;


        }

        void Send_Pong(PingMsg ping)
        {
            UInt16 sender = ping.Src;
            ping.Response = true;

            ping.Src = myAddress;

            byte[] msg = ping.ToBytes();
            myCSMA.Send(sender, msg, 0, (ushort)msg.Length);

        }

        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            PingMsg rcvMsg = new PingMsg(msg, size);

            Send_Pong(rcvMsg);

            Thread.Sleep(100);
            
            Debug.Print("Detection on channel " + PrintChannel(currChannel));

            currChannel = Samraksh.SPOT.Net.Radio.Channels.Channel_11;

            // Reset the channel to 11 and start scanning again
            myCSMA.SetChannel((int)currChannel);

            recvd.SetState(true);
        }

    }

    public class Program
    {
        public static void Main()
        {
            Slave prg = new Slave();

            prg.Initialize();

            prg.Start();

            while (true)
            {
                Thread.Sleep(500);
            }
           
        }

    }
}
