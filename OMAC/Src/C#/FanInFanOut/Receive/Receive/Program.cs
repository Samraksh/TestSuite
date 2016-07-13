#define RF231
//#define SI4468

using System;
using System.IO;
using System.IO.Ports;
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
        const UInt32 displayStats = 100;
#if RF231
        const int initialDelayInMsecs = 30000;
#elif SI4468
        const int initialDelayInMsecs = 75000;
#endif
        const int TIMEBASE = 8000000; // in all power modes, 8 MHz is the timebase
        const int TIMEBASE_MS = TIMEBASE / 1000; // to get answer in milli-seconds
        Hashtable neighborHashtable = new Hashtable();
        //EmoteLCD lcd;
        static readonly EnhancedEmoteLcd Lcd = new EnhancedEmoteLcd();
        
        UInt16 myAddress;
        static UInt32 totalRecvCounter = 0, totalRecvCounterAllElse = 0;

        PingPayload pingMsg = new PingPayload();
        OMAC myOMACObj;
        MACPipe macPipe;

        Parameters p = new Parameters();
        static long testStartTicks;
        static long testEndTicks;
        
        /*private System.IO.Ports.SerialPort _serialPort;
        private readonly byte[] _byteBuffer = new byte[10];
        private static readonly AutoResetEvent GoSemaphore = new AutoResetEvent(false);*/

        public void Initialize()
        {
            //Init LCD
            //lcd = new EmoteLCD();
            //lcd.Initialize();
            //lcd.Write(LCD.CHAR_I, LCD.CHAR_n, LCD.CHAR_i, LCD.CHAR_t);
            Lcd.Display("Init");

            Debug.Print("Initializing radio");
#if RF231
            var radioConfiguration = new RF231RadioConfiguration(RF231TxPower.Power_3dBm, RF231Channel.Channel_13);
#elif SI4468
            var radioConfiguration = new SI4468RadioConfiguration(SI4468TxPower.Power_20dBm, SI4468Channel.Channel_01);
#endif
            
            Debug.Print("Configuring OMAC...");

            try
            {
                //Radio_OMAC_OnBoard = new Radio_802_15_4(RadioUser.OMAC, RadioType.ONBOARDRF231RADIO);
                //configure OMAC
                myOMACObj = new OMAC(radioConfiguration);
                myOMACObj.OnReceive += Receive;
                myOMACObj.OnNeighborChange += NeighborChange;
                
                macPipe = new MACPipe(myOMACObj, PayloadType.Type00);
                macPipe.OnReceive += ReceiveEverythingElse;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("OMAC init done");
            myAddress = myOMACObj.MACRadioObj.RadioAddress;
            Debug.Print("My address is: " + myAddress.ToString() + ". I am in Receive mode");

            testStartTicks = DateTime.Now.Ticks;
            
            //SerialMethod();
        }

        /*public void SerialMethod()
        {
            _serialPort = new SerialPort("COM2")
            {
                BaudRate = 115200,
                Parity = System.IO.Ports.Parity.None,
                DataBits = 8,
                StopBits = StopBits.One,
                Handshake = Handshake.None,
            };
            _serialPort.DataReceived += DataReceived;
            _serialPort.Open();
        }

        /// <summary>
        /// Receive incoming bytes
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            while (_serialPort.BytesToRead > 0)
            {
                var bytesRead = _serialPort.Read(_byteBuffer, 0, System.Math.Max(_serialPort.BytesToRead, _byteBuffer.Length));
                //Debug.Print("BytesToRead: " + _serialPort.BytesToRead + ", bytesRead: " + bytesRead);
                // _bytePacket.Add processes the incoming bytes and calls the user callback when a delimeter is found
                //_bytePacket.Add(_byteBuffer, bytesRead);
                if (string.Equals(_byteBuffer, "GO"))
                {
                    Debug.Print("Received GO");
                    GoSemaphore.Set();
                }
            }
        }*/

        //Keeps track of change in neighborhood
        public void NeighborChange(IMAC macBase, DateTime time)
        {
            //Debug.Print("Count of neighbors " + countOfNeighbors.ToString());
        }

        public void Start()
        {
            Debug.Print("Waiting to start test");
            //GoSemaphore.WaitOne();
            Debug.Print("GO...");
            Thread.Sleep(initialDelayInMsecs);
        }

        public void ReceiveEverythingElse(IMAC macBaseObj, DateTime time, Packet receivedPacket)
        {
            totalRecvCounterAllElse++;
            Debug.Print("---------------------------");
            /*if (myOMACObj.PendingReceivePacketCount() == 0)
            {
                Debug.Print("no packets");
                //return;
                goto endReceiveEverythingElse;
            }

            Packet rcvPacket = myOMACObj.NextPacket();*/
            if (receivedPacket == null)
            {
                Debug.Print("null");
                //return;
                goto endReceiveEverythingElse;
            }
            Debug.Print("Received mac type " + macBaseObj.MACType.ToString());
            Debug.Print("Received payload type " + receivedPacket.PayloadType);
            Debug.Print("totalRecvCounterAllElse is " + totalRecvCounterAllElse);

            byte[] rcvPayload = receivedPacket.Payload;
            if (rcvPayload != null)
            {
                pingMsg = pingMsg.FromBytesToPingPayload(rcvPayload);
                if (pingMsg != null)
                {
                    Debug.Print("Received msgID " + pingMsg.pingMsgId + " from SRC " + receivedPacket.Src);
                    /*NeighborTableInfo nbrTableInfo;
                    //If hashtable already contains an entry for the source, extract it, increment recvCount and store it back
                    if (neighborHashtable.Contains(rcvPacket.Src))
                    {
                        nbrTableInfo = (NeighborTableInfo)neighborHashtable[rcvPacket.Src];
                        nbrTableInfo.recvCount++;
                        nbrTableInfo.AL.Add(pingMsg.pingMsgId);
                        neighborHashtable[rcvPacket.Src] = nbrTableInfo;
                    }
                    //If hashtable does not have an entry, create a new instance and store it
                    else
                    {
                        nbrTableInfo = new NeighborTableInfo();
                        nbrTableInfo.recvCount = 1;
                        ArrayList AL = new ArrayList();
                        AL.Add(pingMsg.pingMsgId);
                        nbrTableInfo.AL = AL;
                        neighborHashtable[rcvPacket.Src] = nbrTableInfo;
                        //neighborHashtable.Add(rcvMsg.Src, nbrTableInfo);
                    }*/
                    //Debug.Print("recvCount from node " + rcvPacket.Src + " is " + nbrTableInfo.recvCount);
                    Debug.Print("Received msgContent " + pingMsg.pingMsgContent.ToString());
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
            //Debug.Print("Received payload type " + macBaseObj.MACConfig.payloadType);

endReceiveEverythingElse:

            Lcd.Display((int)totalRecvCounterAllElse);

            return;
        }

        //Handles received messages 
        public void Receive(IMAC macBaseObj, DateTime time, Packet receivedPacket)
        {
            totalRecvCounter++;
            Debug.Print("---------------------------");
            /*if (myOMACObj.PendingReceivePacketCount() == 0)
            {
                Debug.Print("no packets");
                //return;
                goto endReceive;
            }

            Packet rcvPacket = myOMACObj.NextPacket();*/
            if (receivedPacket == null)
            {
                Debug.Print("null");
                //return;
                goto endReceive;
            }

            Debug.Print("totalRecvCounter is " + totalRecvCounter);

            byte[] rcvPayload = receivedPacket.Payload;
            if (rcvPayload != null)
            {
                pingMsg = pingMsg.FromBytesToPingPayload(rcvPayload);
                if (pingMsg != null)
                {
                    Debug.Print("Received msgID " + pingMsg.pingMsgId + " from SRC " + receivedPacket.Src);
                    NeighborTableInfo nbrTableInfo;
                    //If hashtable already contains an entry for the source, extract it, increment recvCount and store it back
                    if (neighborHashtable.Contains(receivedPacket.Src))
                    {
                        nbrTableInfo = (NeighborTableInfo)neighborHashtable[receivedPacket.Src];
                        nbrTableInfo.recvCount++;
                        nbrTableInfo.AL.Add(pingMsg.pingMsgId);
                        neighborHashtable[receivedPacket.Src] = nbrTableInfo;
                    }
                    //If hashtable does not have an entry, create a new instance and store it
                    else
                    {
                        nbrTableInfo = new NeighborTableInfo();
                        nbrTableInfo.recvCount = 1;
                        ArrayList AL = new ArrayList();
                        AL.Add(pingMsg.pingMsgId);
                        nbrTableInfo.AL = AL;
                        neighborHashtable[receivedPacket.Src] = nbrTableInfo;
                        //neighborHashtable.Add(rcvMsg.Src, nbrTableInfo);
                    }
                    Debug.Print("recvCount from node " + receivedPacket.Src + " is " + nbrTableInfo.recvCount);
                    Debug.Print("Received msgContent " + pingMsg.pingMsgContent.ToString());
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
endReceive:
            testEndTicks = DateTime.Now.Ticks;
            //testEndTime = DateTime.Now;
            //TimeSpan diff = testEndTime.Subtract(testStartTime);
            //There is no diff.TotalMilliseconds
            long duration = testEndTicks - testStartTicks;
            duration = duration / TIMEBASE_MS;
            Debug.Print("Duration: " + duration + " ;" + p.testTimeout);
            if (duration > p.testTimeout)
            {
                Debug.Print("Test timed out");
                TestTimeout();
            }
            else
            {
                if (totalRecvCounter % displayStats == 0)
                {
                    ShowStatistics();
                }
            }

            Lcd.Display((int)totalRecvCounter);

            return;
        }

        void DisplayOnLCD(UInt32 counter)
        {
            Debug.Print("Displaying " + counter + " on LCD");
            /*if (counter < 10)
            {
                lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0, (LCD)counter);
            }
            else if (counter < 100)
            {
                UInt16 tenthPlace = (UInt16)(counter / 10);
                UInt16 unitPlace = (UInt16)(counter % 10);
                lcd.Write(LCD.CHAR_0, LCD.CHAR_0, (LCD)tenthPlace, (LCD)unitPlace);
            }
            else if (counter < 1000)
            {
                UInt16 hundredthPlace = (UInt16)(counter / 100);
                UInt16 remainder = (UInt16)(counter % 100);
                UInt16 tenthPlace = (UInt16)(remainder / 10);
                UInt16 unitPlace = (UInt16)(remainder % 10);
                lcd.Write(LCD.CHAR_0, (LCD)hundredthPlace, (LCD)tenthPlace, (LCD)unitPlace);
            }
            else if (totalRecvCounter < 10000)
            {
                UInt16 thousandthPlace = (UInt16)(counter / 1000);
                UInt16 remainder = (UInt16)(counter % 1000);
                UInt16 hundredthPlace = (UInt16)(remainder / 100);
                remainder = (UInt16)(remainder % 100);
                UInt16 tenthPlace = (UInt16)(remainder / 10);
                UInt16 unitPlace = (UInt16)(remainder % 10);
                lcd.Write((LCD)thousandthPlace, (LCD)hundredthPlace, (LCD)tenthPlace, (LCD)unitPlace);
            }*/
        }

        //Show statistics
        void ShowStatistics()
        {
            Debug.Print("==============STATS================");
            //IEnumerator enumerator = neighborHashtable.GetEnumerator();
            ICollection keyCollection = neighborHashtable.Keys;
            //while (enumerator.MoveNext())
            foreach(ushort nbr in keyCollection)
            {
                //NeighborTableInfo nbrTableInfo = (NeighborTableInfo)enumerator.Current;
                NeighborTableInfo nbrTableInfo = (NeighborTableInfo)neighborHashtable[nbr];
                Debug.Print("Node: " + nbr + "; Total msgs received is " + nbrTableInfo.recvCount);
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
            //Thread.Sleep(Timeout.Infinite);
        }

        public void TestTimeout()
        {
            bool testStatusIndicator = true;
            ICollection keyCollection = neighborHashtable.Keys;
            foreach (ushort nbr in keyCollection)
            {
                NeighborTableInfo nbrTableInfo = (NeighborTableInfo)neighborHashtable[nbr];
                UInt32 lastMsgID = (UInt32)nbrTableInfo.AL[nbrTableInfo.AL.Count-1];
                if (nbrTableInfo.recvCount != lastMsgID)
                {
                    testStatusIndicator = false;
                    string str = "Test fan-in failed for nbr: " + nbr + ". Receive count = " + nbrTableInfo.recvCount + "; Last msgID = " + lastMsgID;
                    DisplayStats(testStatusIndicator, str, null, 0);
                }
            }
            DisplayStats(testStatusIndicator, "Test fan-in successfully completed", null, 0);
        }

        public void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
            while (true)
            {
                Thread.Sleep(1000);
                if (result)
                {
                    Debug.Print("result=PASS\n");
                }
                else
                {
                    Debug.Print("result=FAIL\n");
                }
                Debug.Print("accuracy=" + accuracy.ToString());
                Debug.Print("resultParameter1=" + resultParameter1);
                Debug.Print("resultParameter2=" + resultParameter2);
                Debug.Print("resultParameter3=null");
                Debug.Print("resultParameter4=null");
                Debug.Print("resultParameter5=null");
            }
        }

        public static void Main()
        {
            Program p = new Program();
            p.Initialize();
            //p.Start();
            Thread.Sleep(Timeout.Infinite);
        }
    }
}
