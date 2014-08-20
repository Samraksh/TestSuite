#define HARDWARE_DOTNOW
//#define HARDWARE_ADAPT

using System;
using System.IO.Ports;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;



namespace Level_0A
{
    public struct SamplingParameters
    {
        /// <summary>Number of milliseconds between samples</summary>
        //public const int SamplingIntervalMilliSec = 4000;    // Larger values => fewer samples/sec
        public const int SamplingIntervalMilliSec = 50000;    // Larger values => fewer samples/sec
        /// <summary>Number of samples per second</summary>
        public const int SamplesPerSecond = 1000000 / SamplingIntervalMilliSec;
        /// <summary>Number of microseconds between between samples</summary>
        public const int CallbackIntervalMs = SamplingIntervalMilliSec / 1000;
    }

    public class Program
    {

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

        Timer sampleTimer;

        public static OutputPort Led1, Led2, Led3, Led4;

        private enum PinMap { Gpio01 = 58, Gpio02 = 55, Gpio03 = 53, Gpio04 = 52, Gpio05 = 51 };
        

        Program()
        {
#if HARDWARE_DOTNOW
            Led1 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN1, false);
            Led2 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN2, false);
            Led3 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN3, false);
            Led4 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN4, false);
#elif HARDWARE_ADAPT
            Led1 = new OutputPort((Cpu.Pin)PinMap.Gpio01, false);
            Led2 = new OutputPort((Cpu.Pin)PinMap.Gpio02, false);
            Led3 = new OutputPort((Cpu.Pin)PinMap.Gpio03, false);
            Led4 = new OutputPort((Cpu.Pin)PinMap.Gpio04, false);
#endif
            try
            {
                sampleTimer = new Timer(new TimerCallback(TimerCallbackFunction), null, 0, 100);
            }
            catch (Exception)
            {
                while (true)
                {
                    Thread.Sleep(1000);

                    Debug.Print("result=FAIL\n");
                    Debug.Print("accuracy=0");
                    Debug.Print("resultParameter1=Failed to initialize timer");
                    Debug.Print("resultParameter2=null");
                    Debug.Print("resultParameter3=null");
                    Debug.Print("resultParameter4=null");
                    Debug.Print("resultParameter5=null");
                }
            }
        }

        public static void Main()
        {
            Program pgm = new Program();
            Thread.Sleep(1000);

            /*DateTime prevTime, currentTime;

            Program pgm = new Program();

            while (true)
            {
                ////Thread.Sleep(100);
                prevTime = DateTime.Now;
                Thread.Sleep(50);
                currentTime = DateTime.Now;
                //Debug.Print("currentTime: \n" + currentTime);

                if (currentTime > prevTime)
                {
                    Led1.Write(true);
                    Led1.Write(false);
                }
                else
                {
                    Led2.Write(true);
                    Led2.Write(false);
                }
            }*/
        }

        private static void TimerCallbackFunction(object state)
        {
            //Debug.Print("Inside TimerCallbackFunction\n");
            Led3.Write(true);
            Led3.Write(false);
        }

    }
}


