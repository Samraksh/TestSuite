using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;

namespace Samraksh.APPS.Test
{

    public class NodeStatistics
    {
        public UInt16 src;
        public UInt16 lastRecievedPacketNo;
        public  UInt16 packetsLost;
        public  float expectedPRR;
        public UInt16 totalRecievedPackets;

        public NodeStatistics()
        {
            src = 0;
            lastRecievedPacketNo = 0;
            packetsLost = 0;
            expectedPRR = 0;
            totalRecievedPackets = 0;
        }

        public NodeStatistics(NodeStatistics node)
        {
            this.src = node.src;
            this.lastRecievedPacketNo = node.lastRecievedPacketNo;
            this.packetsLost = node.packetsLost;
            this.expectedPRR = node.expectedPRR;
            this.totalRecievedPackets = 0;
        }

        public void UpdateValues(NodeStatistics node)
        {
            this.totalRecievedPackets++;
            this.src = node.src;
            this.packetsLost = (ushort) ((node.lastRecievedPacketNo - this.totalRecievedPackets));
            this.lastRecievedPacketNo = node.lastRecievedPacketNo;
            this.expectedPRR = (this.packetsLost / this.lastRecievedPacketNo) * 100;
        }

        public void InsertValues(NodeStatistics node)
        {
            this.totalRecievedPackets++;
            this.src = node.src;
            this.packetsLost = 0;
            this.lastRecievedPacketNo = node.lastRecievedPacketNo;
            this.expectedPRR = 0;
        }

    }

    public class NetworkStatistics
    {
        NodeStatistics[] nodeArray;

        public NetworkStatistics()
        {
            nodeArray = new NodeStatistics[10];
            for(int i = 0; i < nodeArray.Length; i++)
            {
                nodeArray[i] = new NodeStatistics();
            }
        }

        public void DisplayStats()
        {
            Debug.Print(" Src ID    |   Packets Lost    |   Last Recieved Packet No. |  PRR     ");
            foreach (NodeStatistics node in nodeArray)
            {
                if (node.src != 0)
                {
                    Debug.Print(node.src.ToString() + "\t|\t" + node.packetsLost.ToString() + "\t|\t" + node.lastRecievedPacketNo.ToString() + "\t\t" + node.expectedPRR.ToString());
                }
            }

        }

        public bool UpdateNode(NodeStatistics incomingNodeData)
        {
            bool nodeExists = false;

            

            for (int i = 0; i < nodeArray.Length; i++)
            {
                
                if (nodeArray[i].src == incomingNodeData.src)
                {
            
                    nodeArray[i].UpdateValues(incomingNodeData);
                    nodeExists = true;
                    break;
                }
                

            }

            

            if (!nodeExists)
            {
                for (int i = 0; i < nodeArray.Length; i++)
                {
                    if (nodeArray[i].src == 0)
                    {
            
                        nodeArray[i].InsertValues(incomingNodeData);
                        break;
                    }
                }
            }

            

            return true;
        }
    }


    public class TestMessage
    {
        private UInt16 seqNo;

        private byte[] convertedToBytes;

        public TestMessage()
        {
            seqNo = 0;
            convertedToBytes = new byte[2];
        }

        public void IncrementSeqNo()
        {
            seqNo++;
        }

        public byte[] ToBytes()
        {
            convertedToBytes[0] = (byte)seqNo;
            convertedToBytes[1] = (byte)(seqNo >> 8);

            return convertedToBytes;
        }

        public ushort getLength()
        {
            return (ushort) convertedToBytes.Length;
        }

    }

    public class PacketLossTest
    {

        byte[] testMessageInBytes;

        public TestMessage tmessage;

        static CSMA myCSMA;
        ReceiveCallBack myReceiveCB;
        NeighbourhoodChangeCallBack myNeighborCB;

        MacConfiguration macConfig = new MacConfiguration();

        System.Threading.Timer displayTimer;

        NetworkStatistics ns;

        NodeStatistics nodeData;


        void NeighborChange(UInt16 noOfNeigbors)
        {
        }

        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            UInt16 currentSeqNo = 0;

            currentSeqNo = msg[0];
            currentSeqNo |= (ushort) (msg[1] << 8);

            nodeData.src = src;
            nodeData.lastRecievedPacketNo = currentSeqNo;

            //Debug.Print("Updating network stats table with" + nodeData.src.ToString() + "\t" + nodeData.lastRecievedPacketNo.ToString());
            ns.UpdateNode(nodeData);
            
            
        }

        void Receive(UInt16 noOfPackets)
        {
            Message rcvMsg = myCSMA.GetNextPacket();

            if (rcvMsg == null)
                return;

            byte[] rcvPayload = rcvMsg.GetMessage();
            
            HandleMessage(rcvPayload, (UInt16)rcvPayload.Length, rcvMsg.Src, rcvMsg.Unicast, rcvMsg.RSSI, rcvMsg.LQI);

        }

        public PacketLossTest()
        {
            tmessage = new TestMessage();


            macConfig.NeighbourLivelinesDelay = 180;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds
            macConfig.radioConfig.SetTxPower(Samraksh.SPOT.Net.Radio.TxPowerValue.Power_0Point7dBm);

            Debug.Print("Configuring:  CSMA...");
            try
            {
                myReceiveCB = Receive;
                myNeighborCB = NeighborChange;
                CSMA.Configure(macConfig, myReceiveCB, myNeighborCB);
                myCSMA = CSMA.Instance;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            ns = new NetworkStatistics();

            nodeData = new NodeStatistics();

        }

        void DisplayTimerCallback(Object state)
        {
            ns.DisplayStats();
        }

        public void Run()
        {

            displayTimer = new System.Threading.Timer(DisplayTimerCallback, null, 1000, 2000);

            while (true)
            {
                tmessage.IncrementSeqNo();

                testMessageInBytes = tmessage.ToBytes();

                myCSMA.Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, testMessageInBytes, (ushort)0, tmessage.getLength());

                System.Threading.Thread.Sleep(500);
            }
        }


        public static void Main()
        {

            Debug.EnableGCMessages(false);

            PacketLossTest plt = new PacketLossTest();

            plt.Run();

        }

    }
}
