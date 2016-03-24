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
        const UInt16 MAX_NEIGHBORS = 12;
        const UInt32 endOfTest = 100;
        const int initialDelayInMsecs = 30000;
        const int TIMEBASE = 8000000; // in all power modes, 8 MHz is the timebase
        const int TIMEBASE_MS = TIMEBASE / 1000; // to get answer in milli-seconds
        Hashtable neighborHashtable = new Hashtable();
        EmoteLCD lcd;
        
        UInt16 myAddress;
        static UInt32 totalRecvCounter = 0;

        //PingPayload pingPayload;
        PingPayload pingMsg = new PingPayload();
        OMAC myOMACObj;

        Parameters p = new Parameters();
        static long testStartTicks;
        static long testEndTicks;
        //static DateTime testStartTime;
        //static DateTime testEndTime;
                
        //ReceiveCallBack myReceiveCB;
        //NeighborhoodChangeCallBack myNeibhborhoodCB;

        //Radio.RadioConfiguration myRadioConfig = new Radio.RadioConfiguration();

        /*private System.IO.Ports.SerialPort _serialPort;
        private readonly byte[] _byteBuffer = new byte[10];
        private static readonly AutoResetEvent GoSemaphore = new AutoResetEvent(false);*/

        public void Initialize()
        {
            //Init LCD
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_n, LCD.CHAR_i, LCD.CHAR_t);

            //myMacConfig.MACRadioConfig = myRadioConfig;
            //Set OMAC parameters
            /*Debug.Print("1.Initializing radio");
            Radio_802_15_4 Radio_OMAC_OnBoard = new Radio_802_15_4();
            Radio_OMAC_OnBoard.TxPowerValue = TxPowerValue.Power_3dBm;
            Radio_OMAC_OnBoard.Channel = Channel.Channel_26;
            Radio_OMAC_OnBoard.RadioType = RadioType.ONBOARDRF231RADIO;
            Radio_OMAC_OnBoard.OnReceiveCallback = Receive;
            Radio_OMAC_OnBoard.OnNeighborChangeCallback = NeighborChange;*/

            Debug.Print("Initializing mac configuration");
            MACConfiguration myMacConfig = new MACConfiguration();
            myMacConfig.NeighborLivenessDelay = 180;
            myMacConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            Debug.Print("2.Initializing radio");
            myMacConfig.MACRadioConfig.TxPower = TxPowerValue.Power_3dBm;
            myMacConfig.MACRadioConfig.Channel = Channel.Channel_26;
            myMacConfig.MACRadioConfig.RadioType = RadioType.RF231RADIO;
            myMacConfig.MACRadioConfig.OnReceiveCallback = Receive;
            myMacConfig.MACRadioConfig.OnNeighborChangeCallback = NeighborChange;

            Debug.Print("Configuring OMAC...");

            try
            {
                //Radio_OMAC_OnBoard = new Radio_802_15_4(RadioUser.OMAC, RadioType.ONBOARDRF231RADIO);
                //configure OMAC
                myOMACObj = new OMAC(myMacConfig);
                
                /*Debug.Print("3.Initializing radio");
                myOMACObj.MACRadioObj.TxPowerValue = TxPowerValue.Power_3dBm;
                myOMACObj.MACRadioObj.Channel = Channel.Channel_26;
                myOMACObj.MACRadioObj.RadioType = RadioType.ONBOARDRF231RADIO;
                myOMACObj.MACRadioObj.OnReceiveCallback = Receive;
                myOMACObj.MACRadioObj.OnNeighborChangeCallback = NeighborChange;*/
                                
                //myReceiveCB = Receive;
                //myNeibhborhoodCB = NeighborChange;
                //macBase = new MACBase(MACType.OMAC);
                //OMAC.Configure(myMacConfig, myReceiveCB, myNeibhborhoodCB);
                //myOMACObj = OMAC.Instance;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("OMAC init done");
            //myAddress = Radio_OMAC_OnBoard.GetRadioAddress();
            myAddress = myOMACObj.GetRadioAddress();
            //myAddress = myOMACObj.MACRadioObj.GetRadioAddress();
            Debug.Print("My address is: " + myAddress.ToString() + ". I am in Receive mode");

            testStartTicks = DateTime.Now.Ticks;
            //testStartTime = DateTime.Now;

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
        public void NeighborChange(UInt16 countOfNeighbors)
        {
            Debug.Print("Count of neighbors " + countOfNeighbors.ToString());
        }

        public void Start()
        {
            Debug.Print("Waiting to start test");
            //GoSemaphore.WaitOne();
            Debug.Print("GO...");
            Thread.Sleep(initialDelayInMsecs);
        }

        //Handles received messages 
        public void Receive(UInt16 countOfPackets)
        {
            totalRecvCounter++;
            Debug.Print("---------------------------");
            if (myOMACObj.GetPendingPacketCount_Receive() == 0)
            {
                Debug.Print("no packets");
                //return;
                goto end;
            }

            Packet rcvPacket = myOMACObj.GetNextPacket();
            if (rcvPacket == null)
            {
                Debug.Print("null");
                //return;
                goto end;
            }

            Debug.Print("totalRecvCounter is " + totalRecvCounter);

            byte[] rcvPayload = rcvPacket.Payload;
            if (rcvPayload != null)
            {
                pingMsg = pingMsg.FromBytesToPingPayload(rcvPayload);
                if (pingMsg != null)
                {
                    Debug.Print("Received msgID " + pingMsg.pingMsgId + " from SRC " + rcvPacket.Src);
                    NeighborTableInfo nbrTableInfo;
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
                    }
                    Debug.Print("recvCount from node " + rcvPacket.Src + " is " + nbrTableInfo.recvCount);
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
end:
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
                if (totalRecvCounter % endOfTest == 0)
                {
                    ShowStatistics();
                }
            }
            return;
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
                    string str = "Test fan-in failed for nbr: " + nbr;
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
