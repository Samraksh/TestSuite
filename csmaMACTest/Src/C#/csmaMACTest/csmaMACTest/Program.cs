using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using System.Collections;
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

    public class TestResult
    {
        public string result { get; set; }
        public string accuracy { get; set; }
        public string resultParameter1 { get; set; }
        public string resultParameter2 { get; set; }
        public string resultParameter3 { get; set; }
        public string resultParameter4 { get; set; }
        public string resultParameter5 { get; set; }
    }

    public class PacketCache
    {
        public int packetSize;
        public byte[] data = new byte[128];
        public ArrayList failList = new ArrayList();
        public ArrayList successList = new ArrayList();

        public PacketCache()
        {
            packetSize = 0;
            for (int i = 0; i < 128; i++)
            {
                data[i] = 0;
            }
        }

        public void CacheIt(int size, byte[] msg)
        {
            packetSize = size;
            msg.CopyTo(data, 0);
        }

        public bool Verify(int size, byte[] msg)
        {
            // Check if the packet size if same otherwise add to failList
            /*
            if (size != packetSize)
            {
                failList.Add(packetSize);
                return false;
            }
             */
            if (msg == null)
            {
                failList.Add(packetSize);
                return false;
            }

            // Check if messages are same
            for (int i = 0; i < packetSize; i++)
            {
                if (msg[i] != data[i])
                {
                    failList.Add(packetSize);
                    return false;
                }
            }

            successList.Add(packetSize);

            return true;
        }

        public void flush()
        {
            packetSize = 0;
            for (int i = 0; i < 128; i++)
            {
                data[i] = 0;
            }
        }

        public ArrayList FailureListIsEmpty()
        {
            if (failList.Count == 0)
                return null;
            else
                return failList;
        }

        public ArrayList GetSuccessList()
        {
            return successList;
        }



    }

    public class csmaMACTest
    {
        Samraksh.SPOT.Net.Mac.CSMA csmaObject = new Net.Mac.CSMA();
        Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new Net.Mac.MacConfiguration();
        ReceiveCallBack receive;
        public static State run = new State();
        public static State response = new State();
        Random rng;
        PacketCache cache;
        int level;
        TestResult result;
        int runs;

        public void Intialize()
        {
            // The total number of runs that happened
            runs = 0;

            // create the result object for communication with the test harness
            result = new TestResult();

            // create a packet cache that stores the last packet that was sent out
            cache = new PacketCache();

            // Configure the mac 
            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            //macConfig.RadioID = (byte) myRadioID;
            macConfig.RadioID = (byte)1;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            // Set the receive handler 
            receive = HandleMessage;


            level = 0;

            // Initially let the test run
            run.SetState(true);

            // Synchronization between send and recieve
            response.SetState(true);

            try
            {
                // Initialize the csma object - interop that actually calls the radio driver
                csmaObject.Initialize(macConfig, receive);
            }
            catch (Exception e)
            {
                throw new TestFailedException("Unable to Initialize CSMA Object");
            }

            // Initializes the random number generator 
            rng = new Random(30);

        }

        // The Level_0 test is designed to send a packet of random size and random data and wait until there is response from the slave mote. 
        // The response is then verified with the cached packet and recorded if there is a failure
        // At this point only the size of the packet that failed is recorded, this then outputted to the test rig
        void Level_0()
        {

            int packetSize = 0;
            byte[] data = new byte[111];

            while (run.IsTrue())
            {
                // If response has not come back in 300ms its likely we never will
                // so consider this as a failure and move on
                if (!response.IsTrue())
                {
                    cache.Verify(0, null);
                    cache.flush();
                    response.SetState(true);
                }

                runs++;

                packetSize = rng.Next(111);
                //packetSize = 112;
                //data = new byte[packetSize];
                rng.NextBytes(data);

                cache.CacheIt(packetSize, data);

                csmaObject.Send((UInt16)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, data, 0, (ushort) packetSize);
                response.SetState(false);

                // Controls the rate of sending 
                Thread.Sleep(300);

            }
        }

        bool ChangeLevel()
        {
            return false;
        }

        void Level_1()
        {
            while (run.IsTrue())
            {
            }
        }

        void Level_2()
        {
            while (run.IsTrue())
            {
            }
        }


        void Run()
        {
            ArrayList fail;
            Level_0();
            if ((fail = cache.FailureListIsEmpty()) == null)
            {
                Debug.Print("result = PASS");
                string successpackets = "";
                ArrayList success = cache.GetSuccessList();
                for (int i = 0; i < success.Count; i++)
                {
                    successpackets += success[i].ToString() + " ";
                }
                float accuracy = success.Count / runs;
                Debug.Print("accuracy = " + accuracy.ToString());
                Debug.Print("resultParameter1 = " + successpackets);
                Debug.Print("resultParameter2 = Total Packets:" + runs);
                
            
            }
            else
            {
                string failedpackets = "";
                for (int i = 0; i < fail.Count; i++)
                {
                    failedpackets += fail[i].ToString() + " ";
                }
                float accuracy = 1 - ((float) fail.Count) / ((float) runs);
                Debug.Print("result = FAIL");
                Debug.Print("accuracy = " + accuracy.ToString());
                Debug.Print("resultParameter1 = Failed Packet Sizes:" + failedpackets);
                Debug.Print("resultParameter2 = Total Packets:" + runs);
            }
        }

        void HandleMessage(byte[] msg, ushort size)
        {
            cache.Verify(msg.Length, msg);
            cache.flush();
            response.SetState(true);

        }



        public static void Main()
        {

            csmaMACTest test = new csmaMACTest();

            try
            {
                test.Intialize();
            }
            catch (TestFailedException)
            {
                throw new TestTerminatedUnSuccessfullyException();
            }
            catch
            {
                throw new TestTerminatedUnSuccessfullyException("Test Terminated due to unknown reasons");
            }

            Timer levelChanger = new Timer(levelChange, null, 40000, 40000);
            
            

            try
            {
                test.Run();
                
            }
            catch
            {
                throw new TestTerminatedUnSuccessfullyException("Test Terminated due to unknown reasons");
            }

        }

      
        public static void levelChange(Object state)
        {
               csmaMACTest.run.SetState(false);
        }


    }
}
