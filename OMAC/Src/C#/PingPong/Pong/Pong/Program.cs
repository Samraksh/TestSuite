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
namespace Samraksh.eMote.Net.Mac.Pong
{
    public class PingPayload
    {
        public UInt32 pingMsgId;
        public string pingMsgContent = "PING";
        public UInt16 pingSenderAddress;

        public PingPayload() { }

        public byte[] ToBytes()
        {
            byte[] msgId = new byte[4];
            msgId[0] = (byte)((pingMsgId >> 24) & 0xFF);
            msgId[1] = (byte)((pingMsgId >> 16) & 0xFF);
            msgId[2] = (byte)((pingMsgId >> 8) & 0xFF);
            msgId[3] = (byte)((pingMsgId) & 0xFF);

            //Convert string to byte array
            byte[] msgContent = System.Text.Encoding.UTF8.GetBytes(pingMsgContent);

            byte[] senderAddress = new byte[2];
            senderAddress[0] = (byte)((pingSenderAddress >> 8) & 0xFF);
            senderAddress[1] = (byte)((pingSenderAddress) & 0xFF);

            //Merge array containing msgID and array containing string into a single byte array for transmission
            byte[] merged = new byte[msgId.Length + msgContent.Length + senderAddress.Length];
            msgId.CopyTo(merged, 0);
            msgContent.CopyTo(merged, msgId.Length);
            senderAddress.CopyTo(merged, msgId.Length + msgContent.Length);

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

                //Convert byte array to ping sender's address
                pingPayload.pingSenderAddress += (UInt16)(msg[8] << 8);
                pingPayload.pingSenderAddress += (UInt16)(msg[9]);

                return pingPayload;
            }
            catch (Exception ex)
            {
                Debug.Print(ex.ToString());
                return null;
            }
        }
    }

    public class PongPayload
    {
        public UInt32 pongMsgId;
        public string pongMsgContent = "PONG";
        public UInt16 pongSenderAddress;
        public UInt16 pongReceiverAddress;

        public PongPayload() { }

        public byte[] ToBytes()
        {
            byte[] msgId = new byte[4];
            msgId[0] = (byte)((pongMsgId >> 24) & 0xFF);
            msgId[1] = (byte)((pongMsgId >> 16) & 0xFF);
            msgId[2] = (byte)((pongMsgId >> 8) & 0xFF);
            msgId[3] = (byte)((pongMsgId) & 0xFF);

            //Convert string to byte array
            byte[] msgContent = System.Text.Encoding.UTF8.GetBytes(pongMsgContent);

            byte[] senderAddress = new byte[2];
            senderAddress[0] = (byte)((pongSenderAddress >> 8) & 0xFF);
            senderAddress[1] = (byte)((pongSenderAddress) & 0xFF);

            byte[] receiverAddress = new byte[2];
            receiverAddress[0] = (byte)((pongReceiverAddress >> 8) & 0xFF);
            receiverAddress[1] = (byte)((pongReceiverAddress) & 0xFF);

            //Merge array containing msgID and array containing string into a single byte array for transmission
            byte[] merged = new byte[msgId.Length + msgContent.Length + senderAddress.Length + receiverAddress.Length];
            msgId.CopyTo(merged, 0);
            msgContent.CopyTo(merged, msgId.Length);
            senderAddress.CopyTo(merged, msgId.Length + msgContent.Length);
            receiverAddress.CopyTo(merged, msgId.Length + msgContent.Length + senderAddress.Length);

            return merged;
        }

        public PongPayload FromBytesToPongPayload(byte[] msg)
        {
            try
            {
                PongPayload pongPayload = new PongPayload();

                //Convert byte array to an integer
                pongPayload.pongMsgId = (UInt32)(msg[0] << 24);
                pongPayload.pongMsgId += (UInt32)(msg[1] << 16);
                pongPayload.pongMsgId += (UInt32)(msg[2] << 8);
                pongPayload.pongMsgId += (UInt32)(msg[3]);

                //Create a byte array to store the string
                byte[] msgContent = new byte[4];
                msgContent[0] = msg[4];
                msgContent[1] = msg[5];
                msgContent[2] = msg[6];
                msgContent[3] = msg[7];

                //Convert byte to char array
                char[] msgContentChar = System.Text.Encoding.UTF8.GetChars(msgContent);

                //Convert char array to string
                pongPayload.pongMsgContent = new string(msgContentChar);

                //Convert byte array to ping sender's address
                pongPayload.pongSenderAddress += (UInt16)(msg[8] << 8);
                pongPayload.pongSenderAddress += (UInt16)(msg[9]);

                //Convert byte array to ping receiver's address
                pongPayload.pongReceiverAddress += (UInt16)(msg[10] << 8);
                pongPayload.pongReceiverAddress += (UInt16)(msg[11]);

                return pongPayload;
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
        const UInt32 totalPingCount = 1000;
        UInt16 myAddress;
        EmoteLCD lcd;
        NetOpStatus status;
        
        static UInt32 sendMsgCounter = 0;
        static UInt32 totalRecvCounter = 0;
        
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
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("OMAC init done");
            myAddress = myOMACObj.GetAddress();
            Debug.Print("My address is: " + myAddress.ToString() + ". I receive pings and send pongs");
        }

        //Keeps track of change in neighborhood
        public void NeighborChange(UInt16 countOfNeighbors)
        {
            Debug.Print("Count of neighbors " + countOfNeighbors.ToString());
        }

        //Handles received messages 
        public void Receive(UInt16 countOfPackets)
        {
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

            byte[] rcvPayload = rcvMsg.GetMessage();
            PingPayload pingPayload = pingMsg.FromBytesToPingPayload(rcvPayload);
            if (pingPayload != null)
            {
                if(pingPayload.pingSenderAddress != 0)
                {
                    totalRecvCounter++;
                    Debug.Print("---------------------------");
                    Debug.Print("totalRecvCounter is " + totalRecvCounter);
                    Debug.Print("Received msgID " + pingPayload.pingMsgId);
                    Debug.Print("Received msgContent " + pingPayload.pingMsgContent.ToString());
                    Debug.Print("Received address " + pingPayload.pingSenderAddress.ToString());
                    Debug.Print("---------------------------");

                    bool result = pingPayload.pingMsgContent.Equals("PING");
                    if (result)
                    {
                        SendPong(pingPayload);
                    }
                    else
                    {
                        Debug.Print("Invalid msg content");
                    }
                }
                else
                {
                    Debug.Print("Received msgID " + pingPayload.pingMsgId + " from sender " + pingPayload.pingSenderAddress + ". Ignoring.");
                }
            }
            else
            {
                Debug.Print("Received a null msg");
            }
        }

        public void SendPong(PingPayload pingPayload)
        {
            sendMsgCounter++;
            pongMsg.pongMsgId = pingPayload.pingMsgId;
            pongMsg.pongSenderAddress = myAddress;
            pongMsg.pongReceiverAddress = pingPayload.pingSenderAddress;
            byte[] msg = pongMsg.ToBytes();

            Debug.Print("Sending to neighbor " + pingPayload.pingSenderAddress + " pong msgID " + pingPayload.pingMsgId);

            status = myOMACObj.Send(pingPayload.pingSenderAddress, msg, 0, (ushort)msg.Length);
            if (status != NetOpStatus.S_Success)
            {
                Debug.Print("Send failed. Pong msgID " + pingPayload.pingMsgId.ToString());
            }

            if (sendMsgCounter == totalPingCount)
            {
                ShowStatistics();
            }
        }

        //Show statistics
        void ShowStatistics()
        {
            Debug.Print("==============STATS================");
            Debug.Print("total msgs sent " + sendMsgCounter);
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