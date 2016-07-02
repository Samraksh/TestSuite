using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using System.Collections;

using Samraksh.eMote.Net;
using Samraksh.eMote.Net.MAC;
using Samraksh.eMote.Net.Radio;
using Samraksh.eMote.DotNow;

//1. This program initializes OMAC as the MAC protocol.
//  1a. Registers a function that tracks change in neighbor (NeighborChange) and a function to handle messages that are received.
//2. Pings are sent at pre-determined intervals.
//3. Pongs are sent back for received messages.
namespace Samraksh.eMote.Net.Mac.Receive
{
    public class NeighborTableInfo
    {
        public UInt32 recvCount;
        public UInt32 prevId;
        public ArrayList AL;
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
                Debug.Print(((UInt32)(msg[0] << 24)).ToString());
                Debug.Print(((UInt32)(msg[1] << 16)).ToString());
                Debug.Print(((UInt32)(msg[2] << 8)).ToString());
                Debug.Print(((UInt32)(msg[3])).ToString());
                Debug.Print(msg[0].ToString());
                Debug.Print(msg[1].ToString());
                Debug.Print(msg[2].ToString());
                Debug.Print(msg[3].ToString());
                Debug.Print(msg[4].ToString());
                Debug.Print(msg[5].ToString());
                Debug.Print(msg[6].ToString());
                Debug.Print(msg[7].ToString());
                return null;
            }
        }
    }

    public class Program
    {
        const UInt16 MAX_NEIGHBORS = 12;
        const UInt32 endOfTest = 50;
        Hashtable neighborHashtable = new Hashtable();
        EmoteLCD lcd;

        UInt16 myAddress;
        static UInt32 totalRecvCounter = 0;

        PingPayload pingMsg = new PingPayload();
        OMAC myOMACObj;

        int errors = 0;


        public void Initialize()
        {
            //Init LCD
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_n, LCD.CHAR_i, LCD.CHAR_t);

            try
            {
                Debug.Print("Initializing radio");
                var radioConfig = new RF231RadioConfiguration(RF231TxPower.Power_0Point0dBm, RF231Channel.Channel_13);

                //configure OMAC
                myOMACObj = new OMAC(radioConfig);
                myOMACObj.OnReceive += Receive;
                myOMACObj.OnNeighborChange += NeighborChange;
				myOMACObj.NeighborLivenessDelay = 20;

                var chan1 = new MACPipe(myOMACObj, PayloadType.Type01);
                chan1.OnReceive += Receive;

                var chan2 = new MACPipe(myOMACObj, PayloadType.Type02);
                chan2.OnReceive += Receive;
            }
            catch (Exception e)
            {
                Debug.Print("exception!: " + e.ToString());
            }

            Debug.Print("OMAC init done");
            myAddress = myOMACObj.MACRadioObj.RadioAddress;
            Debug.Print("My address is: " + myAddress.ToString() + ". I am in Receive mode");
        }

        //Keeps track of change in neighborhood
        public void NeighborChange(IMAC macBase, DateTime time)
        {
            UInt16[] neighborList = OMAC.NeighborListArray();
			DeviceStatus dsStatus = myOMACObj.NeighborList(neighborList);
			int countOfNeighbors = 0;
			for (int j = 0; j<neighborList.Length; j++){
				if (neighborList[j] != 0){
					countOfNeighbors++;
				}
			}
			Debug.Print("************ Neighbor change! New count of neighbors " + countOfNeighbors.ToString());
        }

        //Handles received messages 
        public void Receive(IMAC macBase, DateTime time)
        {
            totalRecvCounter++;
            Debug.Print("---------------------------");
            if (myOMACObj.PendingReceivePacketCount() == 0)
            {
                Debug.Print("no packets");
                return;
            }

            Packet rcvPacket = myOMACObj.NextPacket();
            if (rcvPacket == null)
            {
                Debug.Print("null");
                return;
            }

            Debug.Print("totalRecvCounter is " + totalRecvCounter);

            byte[] rcvPayload = rcvPacket.Payload;
            if (rcvPayload != null)
            {
                PingPayload pingPayload = pingMsg.FromBytesToPingPayload(rcvPayload);
                if (pingPayload != null)
                {
                    Debug.Print("Received msgID " + pingPayload.pingMsgId + " from SRC " + rcvPacket.Src);
                    NeighborTableInfo nbrTableInfo;
                    //If hashtable already contains an entry for the source, extract it, increment recvCount and store it back
                    if (neighborHashtable.Contains(rcvPacket.Src))
                    {
                        NeighborTableInfo nbrTableInfoAnalyze = (NeighborTableInfo)neighborHashtable[rcvPacket.Src];
                        Debug.Print(rcvPacket.Src.ToString() + " " + pingPayload.pingMsgId.ToString() + " " + nbrTableInfoAnalyze.prevId.ToString());
                        if (pingPayload.pingMsgId != nbrTableInfoAnalyze.prevId + 1)
                        {
                            Debug.Print("error");
                            errors++;
                        }

                        nbrTableInfo = (NeighborTableInfo)neighborHashtable[rcvPacket.Src];
                        nbrTableInfo.recvCount++;
                        nbrTableInfo.prevId = pingPayload.pingMsgId;
                        nbrTableInfo.AL.Add(pingPayload.pingMsgId);
                        neighborHashtable[rcvPacket.Src] = nbrTableInfo;

                    }
                    //If hashtable does not have an entry, create a new instance and store it
                    else
                    {
                        nbrTableInfo = new NeighborTableInfo();
                        nbrTableInfo.recvCount = 1;
                        nbrTableInfo.prevId = pingPayload.pingMsgId;
                        ArrayList AL = new ArrayList();
                        AL.Add(pingPayload.pingMsgId);
                        nbrTableInfo.AL = AL;
                        neighborHashtable[rcvPacket.Src] = nbrTableInfo;
                        //neighborHashtable.Add(rcvPacket.Src, nbrTableInfo);
                    }

                    Debug.Print("recvCount from node " + rcvPacket.Src + " is " + nbrTableInfo.recvCount);
                    Debug.Print("Received msgContent " + pingPayload.pingMsgContent.ToString());
                    Debug.Print("---------------------------");
                }
                else
                {
                    Debug.Print("pingPayload is null");
                }
            }
            else
            {
                Debug.Print("Received a null msg");
                Debug.Print(((UInt32)(rcvPayload[0] << 24)).ToString());
                Debug.Print(((UInt32)(rcvPayload[1] << 16)).ToString());
                Debug.Print(((UInt32)(rcvPayload[2] << 8)).ToString());
                Debug.Print(((UInt32)(rcvPayload[3])).ToString());
                Debug.Print(rcvPayload[0].ToString());
                Debug.Print(rcvPayload[1].ToString());
                Debug.Print(rcvPayload[2].ToString());
                Debug.Print(rcvPayload[3].ToString());
                Debug.Print(rcvPayload[4].ToString());
                Debug.Print(rcvPayload[5].ToString());
                Debug.Print(rcvPayload[6].ToString());
                Debug.Print(rcvPayload[7].ToString());
                Debug.Print("---------------------------");
            }
        }

        //Show statistics
        void ShowStatistics()
        {
            UInt32 nbrCnt = 0;
            UInt32 nbr1Cnt = 0;
            UInt32 nbr2Cnt = 0;

            Debug.Print("==============STATS================");
            //IEnumerator enumerator = neighborHashtable.GetEnumerator();
            ICollection keyCollection = neighborHashtable.Keys;
            //while (enumerator.MoveNext())
            foreach (ushort nbr in keyCollection)
            {
                //NeighborTableInfo nbrTableInfo = (NeighborTableInfo)enumerator.Current;
                NeighborTableInfo nbrTableInfo = (NeighborTableInfo)neighborHashtable[nbr];
                Debug.Print("Node: " + nbr + "; Total msgs received is " + nbrTableInfo.recvCount);

                if (nbrCnt == 0)
                {
                    nbrCnt++;
                    nbr1Cnt = nbrTableInfo.recvCount;
                }
                else
                {
                    nbr2Cnt = nbrTableInfo.recvCount;
                }
                Debug.Print("List of msgs: ");
                IEnumerable list = nbrTableInfo.AL;
                foreach (object obj in list)
                {
                    Debug.Print(obj.ToString() + " ,");
                }
                nbrTableInfo.AL.Clear();
            }
            Debug.Print("Total msgs received from all nodes is " + totalRecvCounter);
            Debug.Print("==================================");
            if (errors < (totalRecvCounter*0.05))
            {
                Debug.Print("result = PASS");
                Debug.Print("accuracy = " + errors.ToString());
                Debug.Print("resultParameter1 = " + nbr1Cnt.ToString());
                Debug.Print("resultParameter2 = " + nbr2Cnt.ToString());
                Debug.Print("resultParameter3 = " + totalRecvCounter.ToString());
                Debug.Print("resultParameter4 = null");
                Debug.Print("resultParameter5 = null");
            }
            else
            {
                Debug.Print("result = FAIL");
                Debug.Print("accuracy = " + errors.ToString());
                Debug.Print("resultParameter1 = " + nbr1Cnt.ToString());
                Debug.Print("resultParameter2 = " + nbr2Cnt.ToString());
                Debug.Print("resultParameter3 = " + totalRecvCounter.ToString());
                Debug.Print("resultParameter4 = null");
                Debug.Print("resultParameter5 = null");
            }
            //Thread.Sleep(Timeout.Infinite);
        }

        public static void Main()
        {
            Program p = new Program();
            p.Initialize();
            Thread.Sleep(Timeout.Infinite);
        }
    }
}


