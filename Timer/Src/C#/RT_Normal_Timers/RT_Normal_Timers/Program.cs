#define HARDWARE_EMOTE

using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.RealTime;

namespace TestSuite
{
    public class Program
    {
#if (HARDWARE_EMOTE)
		private static OutputPort testPort_PIN1 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN1, true);
        private static OutputPort testPort_PIN2 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN2, true);
#elif (HARDWARE_ADAPT)
        private enum PinMap { Gpio01 = 58, Gpio02 = 55, Gpio03 = 53, Gpio04 = 52, Gpio05 = 51 };
		private static OutputPort testPort_PIN1 = new OutputPort((Cpu.Pin)PinMap.Gpio01, true);
#endif
		static Samraksh.eMote.RealTime.Timer RT_Timer;

		static System.Threading.Timer sendTimer;
		static bool pinState = false;

		static void sendTimerCallback(Object o)
        {			
            try
            {	
				testPort_PIN2.Write(pinState);
				if (pinState == false)
					pinState = true;
				else 
					pinState = false;
            }
            catch (Exception e)
            {
				Thread.Sleep(1000);
            	
            	Debug.Print("result=FAIL\n");
            	Debug.Print("accuracy=0");
            	Debug.Print("resultParameter1="+e.ToString());
            	Debug.Print("resultParameter2=null");
            	Debug.Print("resultParameter3=null");
            	Debug.Print("resultParameter4=null");
            	Debug.Print("resultParameter5=null");
            }
        }

		private static void RT_TimerCallback(uint data1, uint data2, DateTime time)
        {
			if (pinState == false)
				pinState = true;
			else 
				pinState = false;

            testPort_PIN1.Write(pinState);
        } 

        static NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback);        

        public static void Main()
        {
			Parameters parameters = new Parameters();
			
			int timeTransition = (int)(500 / parameters.frequency);
			try
            {
               sendTimer = new System.Threading.Timer(new TimerCallback(sendTimerCallback), null, 0, timeTransition); 
            }
            catch (Exception)
            {
                while (true){
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

			while (true)
            {
                //The 2nd parameter is in usec.
                RT_Timer = new Samraksh.eMote.RealTime.Timer(10000);
                RT_Timer.OnInterrupt += RT_EventHandler;
                Thread.Sleep(1000);
                Debug.Print("*");
                RT_Timer.Dispose();
                Thread.Sleep(1000);
                Debug.Print("~");
            }
			
        }

    }
}
