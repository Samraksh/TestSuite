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
        Samraksh.SPOT.Net.Mac.CSMA csmaObject = new Net.Mac.CSMA();
        Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new Net.Mac.MacConfiguration();
        ReceiveCallBack receive;
        public static State run = new State();
        

        public void Initialize()
        {
            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            //macConfig.RadioID = (byte) myRadioID;
            macConfig.RadioID = (byte)1;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            receive = HandleMessage;

            // Initially let the test run
            run.SetState(true);

            try
            {
                csmaObject.Initialize(macConfig, receive);
            }
            catch
            {
                throw new TestFailedException("Unable to Initialize CSMA Object");
            }

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


        public void HandleMessage(byte[] msg, ushort size)
        {
            Debug.Print("Recieved message from master\n");

            csmaObject.Send((UInt16)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, msg, 0, (ushort)size);
        }

        public static void Main()
        {
            csmaMACTestSlave test = new csmaMACTestSlave();

            test.Initialize();

            test.Run();
        }

    }
}
