#define HARDWARE_EMOTE

using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;
using Samraksh.eMote.DotNow;

namespace TestSuite
{
    public class Program
    {
#if (HARDWARE_EMOTE)
        private static OutputPort testPort_PIN1 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN1, true);
        private static OutputPort testPort_PIN2 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN2, true);
        private static OutputPort testPort_PIN3 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN3, true);
        private static OutputPort testPort_PIN4 = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN4, true);
#elif (HARDWARE_ADAPT)
        private enum PinMap { Gpio01 = 58, Gpio02 = 55, Gpio03 = 53, Gpio04 = 52, Gpio05 = 51 };
		private static OutputPort testPort_PIN1 = new OutputPort((Cpu.Pin)PinMap.Gpio01, true);
		private static OutputPort testPort_PIN2 = new OutputPort((Cpu.Pin)PinMap.Gpio02, true);
		private static OutputPort testPort_PIN3 = new OutputPort((Cpu.Pin)PinMap.Gpio03, true);
		private static OutputPort testPort_PIN4 = new OutputPort((Cpu.Pin)PinMap.Gpio04, true);
#endif

		static Timer sendTimer1;
		static Timer sendTimer2;
		static Timer sendTimer3;
		static Timer sendTimer4;
		static bool pinState1 = false;
		static bool pinState2 = false;
		static bool pinState3 = false;
		static bool pinState4 = false;

		static void sendTimerCallback1(Object o)
        {			
			testPort_PIN1.Write(pinState1);
			if (pinState1 == false)
				pinState1 = true;
			else 
				pinState1 = false;
        }

		static void sendTimerCallback2(Object o)
        {			
			testPort_PIN2.Write(pinState2);
			if (pinState2 == false)
				pinState2 = true;
			else 
				pinState2 = false;
        }

		static void sendTimerCallback3(Object o)
        {			
			testPort_PIN3.Write(pinState3);
			if (pinState3 == false)
				pinState3 = true;
			else 
				pinState3 = false;
        }

		static void sendTimerCallback4(Object o)
        {			
			testPort_PIN4.Write(pinState4);
			if (pinState4 == false)
				pinState4 = true;
			else 
				pinState4 = false;
        }

        public static void Main()
        {
			Parameters parameters = new Parameters();
			
			int timeTransition1 = (int)(500 / parameters.frequency1);
			int timeTransition2 = (int)(500 / parameters.frequency2);
			int timeTransition3 = (int)(500 / parameters.frequency3);
			int timeTransition4 = (int)(500 / parameters.frequency4);
			try
            {
               sendTimer1 = new Timer(new TimerCallback(sendTimerCallback1), null, 0, timeTransition1); 
               sendTimer2 = new Timer(new TimerCallback(sendTimerCallback2), null, 0, timeTransition2); 
               sendTimer3 = new Timer(new TimerCallback(sendTimerCallback3), null, 0, timeTransition3); 
               sendTimer4 = new Timer(new TimerCallback(sendTimerCallback4), null, 0, timeTransition4); 
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

			while (true){
				//Thread.Sleep(1000);
			}
        }

    }
}
