using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

using Samraksh.eMote.Net;
using Samraksh.eMote.DotNow;

//1. This program initializes OMAC as the MAC protocol.
//  1a. Registers a function that tracks change in neighbor (NeighborChange) and a function to handle messages that are received.
//2. Pings are sent at pre-determined intervals.
//3. Pongs are sent back for received messages.
namespace Samraksh.eMote.Net.Mac.Ping
{
    public class PingPayload
    {
        public UInt32 pingMsgId;
        //public char[] pingMsgContent = new char[5];

        public PingPayload()
        {

        }

        public byte[] ToBytes()
        {
            byte[] msg = new byte[4];
            msg[0] = (byte)((pingMsgId >> 24) & 0xFF);
            msg[1] = (byte)((pingMsgId >> 16) & 0xFF);
            msg[2] = (byte)((pingMsgId >> 8) & 0xFF);
            msg[3] = (byte)((pingMsgId) & 0xFF);
            /*msg[2] = (byte)((pingMsgId >> 8) & 0xFF);
            msg[3] = (byte)((pingMsgId) & 0xFF);*/

            /*msg[4] = (byte)(pingMsgContent[0]);
            msg[5] = (byte)(pingMsgContent[1]);
            msg[6] = (byte)(pingMsgContent[2]);
            msg[7] = (byte)(pingMsgContent[3]);
            msg[8] = (byte)(pingMsgContent[4]);*/
            return msg;
        }

        public PingPayload FromBytesToPingPayload(byte[] msg)
        {
            try
            {
                PingPayload pingPayload = new PingPayload();
                //Debug.Print("================================");
                /*Debug.Print("msg[2] " + (msg[2]));
                Debug.Print("msg[3] " + (msg[3]));*/
                /*Debug.Print("msg[4] " + (msg[4]));
                Debug.Print("msg[5] " + (msg[5]));
                Debug.Print("msg[6] " + (msg[6]));
                Debug.Print("msg[7] " + (msg[7]));
                Debug.Print("msg[8] " + (msg[8]));*/

                /*pingPayload.pingMsgId = (UInt32)(msg[3] << 24);
                pingPayload.pingMsgId += (UInt32)(msg[2] << 16);*/
                pingPayload.pingMsgId = (UInt16)(msg[0] << 24);
                pingPayload.pingMsgId += (UInt16)(msg[1] << 16);
                pingPayload.pingMsgId += (UInt16)(msg[2] << 8);
                pingPayload.pingMsgId += (UInt16)(msg[3]);

                //Debug.Print("================================");

                /*byte[] tmpMsg = new byte[5];
                System.Array.Copy(msg, tmpMsg, 5);

                pingPayload.pingMsgContent = System.Text.Encoding.UTF8.GetChars(tmpMsg);
                Debug.Print("pingMsgContent " + pingPayload.pingMsgContent.ToString());*/

                /*pingPayload.pingMsgContent[0] = System.BitConverter.ToChar(msg, 0);
                Debug.Print("pingPayload.pingMsgContent[0] " + pingPayload.pingMsgContent[0]);
                Debug.Print("pingPayload.pingMsgContent[0] " + pingPayload.pingMsgContent[0].ToString());

                pingPayload.pingMsgContent[1] = System.BitConverter.ToChar(msg, 1);
                Debug.Print("pingPayload.pingMsgContent[1] " + pingPayload.pingMsgContent[1]);
                Debug.Print("pingPayload.pingMsgContent[1] " + pingPayload.pingMsgContent[1].ToString());*/

                /*pingPayload.pingMsgContent[0] = (char)msg[4];
                pingPayload.pingMsgContent[1] = (char)msg[3];
                pingPayload.pingMsgContent[2] = (char)msg[2];
                pingPayload.pingMsgContent[3] = (char)msg[1];
                pingPayload.pingMsgContent[4] = (char)msg[0];*/

                return pingPayload;
            }
            catch (Exception ex)
            {
                Debug.Print(ex.ToString());
                return null;
            }
        }
    }

    public class Program
    {
        //Define nodes that are participating in ping-pong
        UInt16 neighbor1 = 3505;
        UInt16 neighbor2 = 6846;
        
        //public variables
        UInt16 myAddress;
        Timer sendTimer;
        NetOpStatus status;
        static UInt32 sendMsgCounter = 0;
        static UInt32 recvMsgCounter = 1;
        static UInt32 totalRecvCounter = 0;
        EmoteLCD lcd;
        
        PingPayload pingMsg = new PingPayload();
        
        static Mac.OMAC myOMACObj;
        ReceiveCallBack myReceiveCB;
        NeighborhoodChangeCallBack myNeibhborhoodCB;

        Mac.MacConfiguration myMacConfig = new MacConfiguration();
        Radio.RadioConfiguration myRadioConfig = new Radio.RadioConfiguration();
                
        public void Initialize()
        {
            //Init LCD
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_n, LCD.CHAR_i, LCD.CHAR_t);

            //Set OMAC parameters
            myRadioConfig.SetTxPower(Radio.TxPowerValue.Power_3dBm);
            myRadioConfig.SetChannel(Radio.Channels.Channel_26);
            myRadioConfig.SetRadioName(Radio.RadioName.RF231RADIO);

            myMacConfig.radioConfig = myRadioConfig;
            myMacConfig.NeighborLivenessDelay = 180;
            myMacConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            Debug.Print("Configuring OMAC...");

            try
            {
                //configure OMAC
                myReceiveCB = Receive;
                myNeibhborhoodCB = NeighborChange;
                OMAC.Configure(myMacConfig, myReceiveCB, myNeibhborhoodCB);
                myOMACObj = OMAC.Instance;
            }
            catch(Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("OMAC init done");
            myAddress = myOMACObj.GetAddress();
            Debug.Print("My address is: " + myAddress.ToString());
        }

        //Starts a timer 
        public void Start()
        {
            Debug.Print("Starting timer...");
            TimerCallback timerCB = new TimerCallback(sendTimerCallback);
            sendTimer = new Timer(timerCB, null, 0, 5000);
            Debug.Print("Timer initialization done");
        }

        //Calls ping at regular intervals
        void sendTimerCallback(Object obj)
        {
            SendPing();
        }

        //Handles received messages 
        public void Receive(UInt16 countOfPackets)
        {
            totalRecvCounter++;
            Debug.Print("---------------------------");
            if (myOMACObj.GetPendingPacketCount() == 0)
            {
                Debug.Print("no packets");
                return;
            }

            Message rcvMsg = myOMACObj.GetNextPacket();
            if (rcvMsg == null)
            {
                Debug.Print("null");
                return;
            }

            Debug.Print("totalRecvCounter is " + totalRecvCounter);

            byte[] rcvPayload = rcvMsg.GetMessage();
            PingPayload pingPayload = pingMsg.FromBytesToPingPayload(rcvPayload);
            if (pingPayload != null)
            {
                Debug.Print("Received msgID " + pingPayload.pingMsgId);
                while (recvMsgCounter < pingPayload.pingMsgId)
                {
                    Debug.Print("Missed msgID: " + recvMsgCounter);
                    recvMsgCounter++;
                }
                recvMsgCounter = pingPayload.pingMsgId + 1;
                //Debug.Print("Received msgContent " + pingPayload.pingMsgContent.ToString());
            }
            else
            {
                Debug.Print("Received a null msg");
            }

            Debug.Print("---------------------------");
        }

        //Keeps track of change in neighborhood
        public void NeighborChange(UInt16 countOfNeighbors)
        {
            Debug.Print("Count of neighbors " + countOfNeighbors.ToString());
        }

        public void SendPing()
        {
            try
            {
                /*char[] msgChar = new char[5];
                msgChar[0] = 'P';
                msgChar[1] = 'I';
                msgChar[2] = 'N';
                msgChar[3] = 'G';
                msgChar[4] = '\0';
                pingMsg.pingMsgContent = msgChar;*/
                sendMsgCounter++;
                pingMsg.pingMsgId = sendMsgCounter;
                byte[] msg = pingMsg.ToBytes();

                Debug.Print("Sending ping msgID " + sendMsgCounter.ToString());

                if (myAddress == neighbor1)
                {
                    status = myOMACObj.Send(neighbor2, msg, 0, (ushort)msg.Length);
                }
                else
                {
                    status = myOMACObj.Send(neighbor1, msg, 0, (ushort)msg.Length);
                }

                if (status != NetOpStatus.S_Success)
                {
                    Debug.Print("Send failed. Ping msgID " + sendMsgCounter.ToString());
                }

                if (sendMsgCounter < 10) 
                {
                    lcd.Write(LCD.CHAR_S, LCD.CHAR_S, LCD.CHAR_S, (LCD)sendMsgCounter);
                }
                else if (sendMsgCounter < 100)
                {
                    UInt16 tenthPlace = (UInt16)(sendMsgCounter / 10);
                    UInt16 unitPlace = (UInt16)(sendMsgCounter % 10);
                    lcd.Write(LCD.CHAR_S, LCD.CHAR_S, (LCD)tenthPlace, (LCD)unitPlace);
                }
                else if (sendMsgCounter < 1000)
                {
                    UInt16 hundredthPlace = (UInt16)(sendMsgCounter / 100);
                    UInt16 remainder = (UInt16)(sendMsgCounter % 100);
                    UInt16 tenthPlace = (UInt16)(remainder / 10);
                    UInt16 unitPlace = (UInt16)(remainder % 10);
                    lcd.Write(LCD.CHAR_S, (LCD)hundredthPlace, (LCD)tenthPlace, (LCD)unitPlace);
                }
                else if (sendMsgCounter < 10000)
                {
                    UInt16 thousandthPlace = (UInt16)(sendMsgCounter / 1000);
                    UInt16 remainder = (UInt16)(sendMsgCounter % 1000);
                    UInt16 hundredthPlace = (UInt16)(remainder / 100);
                    remainder = (UInt16)(remainder % 100);
                    UInt16 tenthPlace = (UInt16)(remainder / 10);
                    UInt16 unitPlace = (UInt16)(remainder % 10);
                    lcd.Write((LCD)thousandthPlace, (LCD)hundredthPlace, (LCD)tenthPlace, (LCD)unitPlace);
                }
            }
            catch (Exception ex)
            {
                Debug.Print("SendPing: " + ex.ToString());
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
