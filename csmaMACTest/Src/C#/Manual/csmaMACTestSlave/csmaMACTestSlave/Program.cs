using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Microsoft.SPOT.Hardware;


namespace Samraksh.SPOT.Test
{
    public class State
    {
        private readonly Object _lock = new Object();
        private bool ready;

        public State()
        {
            ready = false;
        }

        public bool IsTrue()
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

    public class csmaMACTestSlave
    {
        static Samraksh.SPOT.Net.Mac.CSMA myCSMA;
        ReceiveCallBack myReceiveCB;
        NeighbourhoodChangeCallBack myNeighborCB;
        Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new Net.Mac.MacConfiguration();

        //Samraksh.SPOT.Net.Mac.CSMA csmaObject = new Net.Mac.CSMA();
        public static State run = new State();

        byte[] data = new byte[111];
        int packetSize = 0;
        Random rng;
        

        public void Initialize()
        {
            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            //macConfig.RadioID = (byte) myRadioID;
            macConfig.RadioID = (byte)1;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds
            macConfig.NeighbourLivelinesDelay = 180;
            macConfig.radioConfig.SetTxPower(Samraksh.SPOT.Net.Radio.TxPowerValue.Power_0Point7dBm);

            // Set the receive handler 
            myReceiveCB = HandleMessage;

            // Set the neighbor change handler
            myNeighborCB = NeighborChange;

            // Initially let the test run
            run.SetState(true);

            Debug.Print("Configuring:  CSMA...");
            try
            {
                Net.Mac.CSMA.Configure(macConfig, myReceiveCB, myNeighborCB);
                myCSMA = Net.Mac.CSMA.Instance;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            // Initializes the random number generator 
            rng = new Random(30);

        }


        void Level_0()
        {
            while (run.IsTrue())
            {
                Thread.Sleep(1000);
            }
        }

        void Run()
        {
            Level_0();
        }

        void NeighborChange(UInt16 noOfNeigbors)
        {
        }

        //public void HandleMessage(byte[] msg, ushort size)
        public void HandleMessage(ushort NumberOfPacketsReceived)
        {
            Debug.Print("Recieved message from master\n");
            packetSize = rng.Next(111);
            rng.NextBytes(data);

            myCSMA.Send((UInt16)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, data, 0, (ushort)packetSize);
        }

        public static void Main()
        {
            csmaMACTestSlave test = new csmaMACTestSlave();

            test.Initialize();

            test.Run();
        }

    }
}
