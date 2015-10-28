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
        public char[] pingMsgContent = new char[5];

        public PingPayload()
        {

        }

        public byte[] ToBytes()
        {
            byte[] msg = new byte[9];
            msg[0] = (byte)((pingMsgId >> 24) & 0xFF);
            msg[1] = (byte)((pingMsgId >> 16) & 0xFF);
            msg[2] = (byte)((pingMsgId >> 8) & 0xFF);
            msg[3] = (byte)((pingMsgId) & 0xFF);

            msg[4] = (byte)(pingMsgContent[0]);
            msg[5] = (byte)(pingMsgContent[1]);
            msg[6] = (byte)(pingMsgContent[2]);
            msg[7] = (byte)(pingMsgContent[3]);
            msg[8] = (byte)(pingMsgContent[4]);
            return msg;
        }
    }

    public class PongPayload
    {
        public UInt32 pongMsgId;
        public string pongMsgContent;

        public PongPayload()
        {

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
        static UInt32 msgCounter = 0;
        EmoteLCD lcd;
        
        PingPayload pingMsg = new PingPayload();
        PongPayload pongMsg = new PongPayload();

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
            sendTimer = new Timer(timerCB, null, 0, 400);
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
                msgCounter++;
                char[] msgChar = new char[5];
                msgChar[0] = 'P';
                msgChar[1] = 'I';
                msgChar[2] = 'N';
                msgChar[3] = 'G';
                msgChar[4] = '\0';
                pingMsg.pingMsgId = msgCounter;
                pingMsg.pingMsgContent = msgChar;
                byte[] msg = pingMsg.ToBytes();

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
                    Debug.Print("Send failed. Ping msgID " + msgCounter.ToString());
                }

                if (msgCounter < 10) 
                { 
                    lcd.Write(LCD.CHAR_S, LCD.CHAR_S, LCD.CHAR_S, (LCD)msgCounter);
                }
                else if (msgCounter < 100)
                {
                    uint tenthPlace = msgCounter / 10;
                    uint unitPlace = msgCounter % 10;
                    lcd.Write(LCD.CHAR_S, LCD.CHAR_S, (LCD)tenthPlace, (LCD)unitPlace);
                }
                else if (msgCounter < 1000)
                {
                    uint hundredthPlace = msgCounter / 100;
                    uint remainder = msgCounter % 100;
                    uint tenthPlace = remainder / 10;
                    uint unitPlace = remainder % 10;
                    lcd.Write(LCD.CHAR_S, (LCD)hundredthPlace, (LCD)tenthPlace, (LCD)unitPlace);
                }
                else if (msgCounter < 10000)
                {
                    uint thousandthPlace = msgCounter / 1000;
                    uint remainder = msgCounter % 1000;
                    uint hundredthPlace = remainder / 100;
                    remainder = remainder % 100;
                    uint tenthPlace = remainder / 10;
                    uint unitPlace = remainder % 10;
                    lcd.Write((LCD)thousandthPlace, (LCD)hundredthPlace, (LCD)tenthPlace, (LCD)unitPlace);
                }
            }
            catch (Exception ex)
            {
                Debug.Print("SendPing: " + ex.ToString());
            }
        }

        public void SendPong()
        {
            
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
