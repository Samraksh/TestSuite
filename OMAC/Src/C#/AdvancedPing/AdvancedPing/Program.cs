using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

using Samraksh.eMote.Net;
using Samraksh.eMote.DotNow;

namespace Samraksh.eMote.Net.Mac.AdvancedPing
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
            //return PingMsg.Size();
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

    public class Program
    {
        const UInt16 MAX_NEIGHBORS = 12;
        UInt16 dutyCyclePeriod = 5000;

        static UInt32 totalRecvCounter = 0;
        bool startSend = false;
        NetOpStatus status;
        const int firstPos = 10;    //First position in rxBuffer from which verification is made to see if a msg reached
        const int testCount = 50;   //Defines how many msgs to compare and store
        UInt16 myAddress;
        UInt16 mySeqNo = 1;         //The send msgID. Incremented by after every send
        UInt16 receivePackets = 0;  //keeps track of total msgs received
        UInt16 lastRxSeqNo = 0;
        ushort[] rxBuffer = new ushort[testCount];  //stores testCount number of received msgIDs
        Timer sendTimer;
        EmoteLCD lcd;
        PingMsg sendMsg = new PingMsg();
        
        //Random rand = new Random();
        //Radio.Radio_802_15_4 my_15_4 = new Radio.Radio_802_15_4();
        //Radio.RadioConfiguration radioConfig = new Radio.RadioConfiguration();
        //int myRadioID;

        static Mac.OMAC myOMACObj;
        ReceiveCallBack myReceiveCB;
        NeighborhoodChangeCallBack myNeighborCB;

        Mac.MacConfiguration macConfig = new MacConfiguration();

        void Initialize()
        {
            Debug.Print("Initializing:  EmotePingwLCD");
            Thread.Sleep(1000);
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_N, LCD.CHAR_I, LCD.CHAR_7);

            macConfig.NeighborLivenessDelay = 180;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            Debug.Print("Configuring:  OMAC...");
            try
            {
                myReceiveCB = Receive;
                myNeighborCB = NeighborChange;
                OMAC.Configure(macConfig, myReceiveCB, myNeighborCB);
                myOMACObj = OMAC.Instance;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("OMAC Init done.");
            myAddress = myOMACObj.GetAddress();
            Debug.Print("My default address is :  " + myAddress.ToString());
        }

        void NeighborChange(UInt16 noOfNeighbors)
        {
            Debug.Print("neighbor count: " + noOfNeighbors.ToString());
        }

        void Start()
        {
            Debug.Print("Starting timer...");
            sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, dutyCyclePeriod);
            Debug.Print("Timer init done.");
        }

        void sendTimerCallback(Object o)
        {
            // We receieved enough data....looking to see if we received all packets (even in best case scenario we could have a few errors)
            // we wait a bit longer just so the other side will also receive enough packets
            /*if (receivePackets >= (testCount + 20))
            {
                int i, j;
                bool found = false;
                int errors = 0;
                // searching for testCount-20 numbers starting with the first number we received
                ushort comparisonValue = rxBuffer[firstPos];
                for (i = firstPos; i < testCount - 20; i++)
                {
                    found = false;
                    for (j = firstPos; j < testCount; j++)
                    {
                        if (rxBuffer[j] == comparisonValue + 1)
                        {
                            found = true;
                        }
                    }
                    if (found != true)
                    {
                        Debug.Print("couldn't find " + (comparisonValue + 1).ToString() + " @ " + i.ToString() + " found " + rxBuffer[i].ToString());
                        errors++;
                    }
                    comparisonValue = rxBuffer[i];
                }
                if ((receivePackets % 5) == 0)
                {
                    if (errors < 6)
                    {
                        Debug.Print("result = PASS");
                        Debug.Print("accuracy = " + errors.ToString());
                        Debug.Print("resultParameter1 = " + rxBuffer[firstPos].ToString());
                        Debug.Print("resultParameter2 = " + rxBuffer[testCount - 1].ToString());
                        Debug.Print("resultParameter3 = " + receivePackets.ToString());
                        Debug.Print("resultParameter4 = null");
                        Debug.Print("resultParameter5 = null");
                    }
                    else
                    {
                        Debug.Print("result = FAIL");
                        Debug.Print("accuracy = " + errors.ToString());
                        Debug.Print("resultParameter1 = " + rxBuffer[firstPos].ToString());
                        Debug.Print("resultParameter2 = " + rxBuffer[testCount - 1].ToString());
                        Debug.Print("resultParameter3 = " + receivePackets.ToString());
                        Debug.Print("resultParameter4 = null");
                        Debug.Print("resultParameter5 = null");
                    }
                }
            }*/
            try
            {
                Send_Ping(sendMsg);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }

        //Keeps sending a ping once a period defined by the timer's dutyCyclePeriod
        void Send_Ping(PingMsg ping)
        {
            try
            {
                Debug.GC(true);
                bool sendFlag = false;
                
                UInt16[] neighborList = myOMACObj.GetNeighborList();
                //if (neighborList.Length > 0)
                //{
                    for (int i = 0; i < MAX_NEIGHBORS; i++)
                    {
                        if (neighborList[i] != 0)
                        {
                            //Debug.Print("count of neighbors " + neighborList.Length);
                            startSend = true;
                            sendFlag = true;
                            Debug.Print("Sending to neighbor " + neighborList[i] + " ping msgID " + mySeqNo);

                            ping.Response = false;
                            ping.MsgID = mySeqNo++;
                            ping.Src = myAddress;
                            byte[] msg = ping.ToBytes();
                            status = myOMACObj.Send(neighborList[i], msg, 0, (ushort)msg.Length);
                            if (status != NetOpStatus.S_Success)
                            {
                                Debug.Print("Failed to send: " + ping.MsgID.ToString());
                            }

                            int char0 = (mySeqNo % 10) + (int)LCD.CHAR_0;
                            lcd.Write(LCD.CHAR_S, LCD.CHAR_S, LCD.CHAR_S, (LCD)char0);
                        }
                    }
                    if (sendFlag == false && startSend == true)
                    {
                        Debug.Print("Ping failed. All neighbors dropped out");
                    }
                //}

                /*if (myAddress == neighbor1)
                {
                    status = myOMACObj.Send(neighbor2, msg, 0, (ushort)msg.Length);
                }
                else
                {
                    status = myOMACObj.Send(neighbor1, msg, 0, (ushort)msg.Length);
                }*/

                
            }
            catch (Exception e)
            {
                Debug.Print("Send_Ping:" + e.ToString());
            }
        }

        //Handles the received messages by passing control over to HandleMessage
        void Receive(UInt16 noOfPackets)
        {
            if (myOMACObj.GetPendingPacketCount() == 0)
            {
                Debug.Print("no packets");
                return;
            }

            //while (myOMACObj.GetPendingPacketCount() > 0) {
            Message rcvMsg = myOMACObj.GetNextPacket();
            if (rcvMsg == null)
            {
                Debug.Print("null");
                return;
            }

            //Debug.Print("Received a msg from " + rcvMsg.Src + " of size " + rcvMsg.Size);
            byte[] rcvPayload = rcvMsg.GetMessage();
            
            HandleMessage(rcvPayload, (UInt16)rcvMsg.Size, rcvMsg.Src, rcvMsg.Unicast, rcvMsg.RSSI, rcvMsg.LQI);
        }

        //Parses received msg and send a pong in response to the ping
        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            try
            {
                Debug.GC(true);
                /*if (unicast)
                {
                    Debug.Print("Got a Unicast message from src: " + src.ToString() + ", size: " + size.ToString() + ", rssi: " + rssi.ToString() + ", lqi: " + lqi.ToString());
                }
                else
                {
                    Debug.Print("Got a broadcast message from src: " + src.ToString() + ", size: " + size.ToString() + ", rssi: " + rssi.ToString() + ", lqi: " + lqi.ToString());
                }*/
                //if (size == PingMsg.Size())
                //{
                    //Debug.Print("MSG: " + msg[0].ToString() + " " + msg[1].ToString() + " " + msg[2].ToString() + " " + msg[3].ToString() + " " + msg[4].ToString() + " " + msg[5].ToString());
                    Debug.Print("========================================");
                    totalRecvCounter++;
                    Debug.Print("totalRecvCounter is " + totalRecvCounter);
                    PingMsg rcvMsg = new PingMsg(msg, size);

                    if (rcvMsg.Response)
                    {
                        if (receivePackets < testCount)
                        {
                            //This is a response to my message						
                            rxBuffer[receivePackets] = rcvMsg.MsgID;
                        }
                        receivePackets++;
                        Debug.Print("Received response from: " + rcvMsg.Src.ToString() + " for seq no: " + rcvMsg.MsgID.ToString() + " of size " + size);
                        lcd.Write(LCD.CHAR_P, LCD.CHAR_P, LCD.CHAR_P, LCD.CHAR_P);
                    }
                    else
                    {
                        //if ( ((UInt16)rcvMsg.MsgID) != lastRxSeqNo + 1)
                        //	Debug.Print("***** Missing seq no: " + (lastRxSeqNo + 1).ToString() + " *****");
                        Debug.Print("Sending a Pong to SRC: " + rcvMsg.Src.ToString() + " for seq no: " + rcvMsg.MsgID.ToString());
                        lastRxSeqNo = (UInt16)rcvMsg.MsgID;
                        lcd.Write(LCD.CHAR_R, LCD.CHAR_R, LCD.CHAR_R, LCD.CHAR_R);
                        Send_Pong(rcvMsg);
                    }
                    Debug.Print("========================================");
                /*}
                else
                {
                    Debug.Print("not proper size with possible ID of: " + ((UInt16)(msg[1] << 8)).ToString());
                }*/
            }
            catch (Exception e)
            {
                Debug.Print("HandleMessage:" + e.ToString());
            }
        }

        //Msg with same ID as ping is sent back as pong to sender, but with the response flag set to true.
        void Send_Pong(PingMsg ping)
        {
            try
            {
                UInt16 sender = ping.Src;
                ping.Response = true;

                ping.Src = myAddress;

                byte[] msg = ping.ToBytes();
                status = myOMACObj.Send(sender, msg, 0, (ushort)msg.Length);
                if (status != NetOpStatus.S_Success)
                {
                    Debug.Print("Failed to send: " + ping.MsgID.ToString());
                }
            }
            catch (Exception e)
            {
                Debug.Print("Send_Pong:" + e.ToString());
            }
        }

        public static void Main()
        {
            Program p = new Program();
            p.Initialize();
            p.Start();
            Thread.Sleep(Timeout.Infinite);
        }
    }
}


