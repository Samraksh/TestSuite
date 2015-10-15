using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;
//using Samraksh.eMote.RealTime;
using Samraksh.eMote.RealTime;
using Samraksh.eMote.DotNow;

namespace TestSuite
{
    public class Program
    {
#if (HARDWARE_EMOTE)
        private static OutputPort testPort_PIN1 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN1, true);
#elif (HARDWARE_ADAPT)
        private enum PinMap { Gpio01 = 58, Gpio02 = 55, Gpio03 = 53, Gpio04 = 52, Gpio05 = 51 };
		//private static OutputPort testPort_55 = new OutputPort((Cpu.Pin)55, true);
		private static OutputPort testPort_PIN1 = new OutputPort((Cpu.Pin)PinMap.Gpio01, true);
		/*private static OutputPort testPort_PA1 = new OutputPort(GPIOPins.GPIO_PIN_PA1, true);
		private static OutputPort testPort_PA2 = new OutputPort(GPIOPins.GPIO_PIN_PA2, true);
		private static OutputPort testPort_PA3 = new OutputPort(GPIOPins.GPIO_PIN_PA3, true);
		private static OutputPort testPort_PA4 = new OutputPort(GPIOPins.GPIO_PIN_PA4, true);
		private static OutputPort testPort_PA5 = new OutputPort(GPIOPins.GPIO_PIN_PA5, true);
		private static OutputPort testPort_PA6 = new OutputPort(GPIOPins.GPIO_PIN_PA6, true);
		private static OutputPort testPort_PA7 = new OutputPort(GPIOPins.GPIO_PIN_PA7, true);
		private static OutputPort testPort_PA8 = new OutputPort(GPIOPins.GPIO_PIN_PA8, true);
		private static OutputPort testPort_PA9 = new OutputPort(GPIOPins.GPIO_PIN_PA9, true);*/
#endif

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

        private static void RT_TimerCallback(uint data1, uint data2, DateTime time)
        {
			if (pinState == false)
				pinState = true;
			else 
				pinState = false;

            testPort_PIN1.Write(pinState);
        } 

		static Samraksh.eMote.RealTime.Timer RT_Timer;
		static NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback);
		static bool pinState = false;

        public static void Main()
        {
			Parameters parameters = new Parameters();
			
			ulong timeTransition = (ulong)(500000 / parameters.frequency);
			try
            {
                //The 2nd parameter is in usec.
                RT_Timer = new Samraksh.eMote.RealTime.Timer(timeTransition);
                RT_Timer.OnInterrupt += RT_EventHandler;
            }
            catch (Exception)
            {
                while (true){
					Thread.Sleep(1000);
            	
            	    Debug.Print("result=FAIL\n");
            		Debug.Print("accuracy=0");
            		Debug.Print("resultParameter1=Failed to initialize RealTimer timer");
            		Debug.Print("resultParameter2=null");
            		Debug.Print("resultParameter3=null");
            		Debug.Print("resultParameter4=null");
            		Debug.Print("resultParameter5=null");
				}
            }

			while (true){
				Thread.Sleep(1000);
			}
        }

    }
}
