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
namespace Samraksh.eMote.Net.Mac.FanIn
{
    public class NeighborTableInfo
    {
        public UInt32 recvCount;
    }

    public class PingPayload
    {
        public UInt32 pingMsgId;
        public string pingMsgContent = "PING";

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

            //Convert string to byte array
            byte[] msgContent = System.Text.Encoding.UTF8.GetBytes(pingMsgContent);

            //Merge array containing msgID and array containing string into a single byte array for transmission
            byte[] merged = new byte[msg.Length + msgContent.Length];
            msg.CopyTo(merged, 0);
            msgContent.CopyTo(merged, msg.Length);

            return merged;
        }

        public PingPayload FromBytesToPingPayload(byte[] msg)
        {
            try
            {
                PingPayload pingPayload = new PingPayload();

                //Convert byte array to an integer
                pingPayload.pingMsgId = (UInt32)(msg[0] << 24);
                pingPayload.pingMsgId += (UInt32)(msg[1] << 16);
                pingPayload.pingMsgId += (UInt32)(msg[2] << 8);
                pingPayload.pingMsgId += (UInt32)(msg[3]);

                //Create a byte array to store the string
                byte[] msgContent = new byte[4];
                msgContent[0] = msg[4];
                msgContent[1] = msg[5];
                msgContent[2] = msg[6];
                msgContent[3] = msg[7];

                //Convert byte to char array
                char[] msgContentChar = System.Text.Encoding.UTF8.GetChars(msgContent);

                //Convert char array to string
                pingPayload.pingMsgContent = new string(msgContentChar);

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
        //public variables
        //UInt32 totalPingCount = 1000;
        const UInt16 MAX_NEIGHBORS = 12;

        System.Collections.Hashtable neighborHashtable = new System.Collections.Hashtable();
        
        UInt16 myAddress;
        static UInt32 recvMsgCounter = 0;
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
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("OMAC init done");
            myAddress = myOMACObj.GetAddress();
            Debug.Print("My address is: " + myAddress.ToString() + ". I am in FanIn mode");
        }

        //Keeps track of change in neighborhood
        public void NeighborChange(UInt16 countOfNeighbors)
        {
            Debug.Print("Count of neighbors " + countOfNeighbors.ToString());
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
                Debug.Print("Received msgID " + pingPayload.pingMsgId + " from SRC " + rcvMsg.Src);
                NeighborTableInfo nbrTableInfo;
                //If hashtable already contains an entry for the source, extract it, increment recvCount and store it back
                if (neighborHashtable.Contains(rcvMsg.Src))
                {
                    nbrTableInfo = (NeighborTableInfo)neighborHashtable[rcvMsg.Src];
                    nbrTableInfo.recvCount++;
                    neighborHashtable[rcvMsg.Src] = nbrTableInfo;
                }
                //If hashtable does not have an entry, create a new instance and store it
                else
                {
                    nbrTableInfo = new NeighborTableInfo();
                    nbrTableInfo.recvCount = 1;
                    neighborHashtable[rcvMsg.Src] = nbrTableInfo;
                    //neighborHashtable.Add(rcvMsg.Src, nbrTableInfo);
                }
                nbrTableInfo = (NeighborTableInfo)neighborHashtable[rcvMsg.Src];
                //while (recvMsgCounter < pingPayload.pingMsgId)
                recvMsgCounter = nbrTableInfo.recvCount + 1;
                while (recvMsgCounter < nbrTableInfo.recvCount)
                {
                    Debug.Print("Missed msgID: " + recvMsgCounter);
                    recvMsgCounter++;
                }
                //recvMsgCounter = pingPayload.pingMsgId + 1;
                Debug.Print("Received msgContent " + pingPayload.pingMsgContent.ToString());
            }
            else
            {
                Debug.Print("Received a null msg");
            }

            Debug.Print("---------------------------");
        }

        //Show statistics
        void ShowStatistics()
        {
            Debug.Print("==============STATS================");
            Debug.Print("total msgs received " + totalRecvCounter);
            Debug.Print("==================================");
            Thread.Sleep(Timeout.Infinite);
        }

        public static void Main()
        {
            Program p = new Program();
            p.Initialize();
            Thread.Sleep(Timeout.Infinite);
        }
    }
}
