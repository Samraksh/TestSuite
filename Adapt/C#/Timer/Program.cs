using System;
using Microsoft.SPOT;
using Samraksh.eMote.DotNow;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace HeartRate
{
    public class Program
    {
        private static AutoResetEvent autoEvent = new AutoResetEvent(false);
        // this will block any thread calling the WaitOne () method

        const int ARRAY_SIZE = 100;
        const int DUMMY_THREADS = 1;
        private static Int64[] tArray = new Int64[ARRAY_SIZE];
        private static int tArrayCtr = 0;

        private static class DummyWork {
            public static bool Done;
            private static Int16 cnt = 0;

            public static void MakeWork() {
                Debug.Print("Dummy MakeWork started");
                while (false == Done) {
                    Debug.Print("Running dummy cnt="+ cnt++);
                    Thread.Sleep(100);
                }     // loop endlessly creating work.
                cnt++;
            } // MakeWork
        }

        // On timer tick, check if done, then add current time to array.
        private static void TimerTick(object rv) {
            Debug.Print("Inside TimerTick tArrayCtr " + tArrayCtr);

            // Check first to avoid race condition
            if (tArrayCtr >= tArray.Length) {
                autoEvent.Set();            // set autoEvent to true & unblock main
                DummyWork.Done = true;
                return;
            }

            tArray[tArrayCtr] = DateTime.Now.Ticks;
            tArrayCtr++;
        }

        public static void Main() {
            int i;
            Debug.Print("Main Started");

            // you can try using the debugger if you want
            // in that case, you'll need this sleep statement so you'll have time to connect
            // otherwise, comment this out;
            // Thread.Sleep(10*1000);            // sleep 10 seconds
            Debug.Print("Thread.Sleep done, starting the dummy threads");

            //// spawn the dummy threads.  They die when DummyWork.done = true;
            //var dummy = new Thread[DUMMY_THREADS];
            //for (i = 0; i < DUMMY_THREADS; i++) {
            //    Debug.Print("Dummy thread " + i);
            //    dummy[i] = new Thread(DummyWork.MakeWork);
            //    dummy[i].Start();
            //}

            (new Thread(DummyWork.MakeWork)).Start();

            // Start a periodic timer.  period is 1000.
            // Shorter periods demonstrates jitter better, up to a limit.
            var t = new Timer(TimerTick, null, 0, 1000);
            Debug.Print("Timer started on TimerTick");

            // main thread
            autoEvent.WaitOne();                // wait for event to be true
            Debug.Print("autoEvent.WaitOne notification received");

            t.Dispose();                        // dispose timer

            // print tArray
            Debug.Print("Printing data ");
            for (i = 0; i < ARRAY_SIZE; i++) {
                Debug.Print(tArray[i] + ",");
            }
            // compute stats: min, max, mean, std: Do this on the csv file
            // Use code at http://www.codeproject.com/KB/cs/csstatistics.aspx
        }
    }
}
