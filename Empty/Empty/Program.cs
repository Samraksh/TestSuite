using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
//using Samraksh.SPOT.Hardware.EmoteDotNow;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
		static Timer sendTimer1;
		static Timer sendTimer2;
static bool pinState1 = false;
		static bool pinState2 = false;
		static void sendTimerCallback1(Object o)
        {			
			if (pinState1 == false)
				pinState1 = true;
			else 
				pinState1 = false;
        }

		static void sendTimerCallback2(Object o)
        {			
			if (pinState2 == false)
				pinState2 = true;
			else 
				pinState2 = false;
        }

        public static void Main()
        {
			//Thread.Sleep(Timeout.Infinite);
			sendTimer1 = new Timer(new TimerCallback(sendTimerCallback1), null, 0, 15); 
               sendTimer2 = new Timer(new TimerCallback(sendTimerCallback2), null, 0, 16); 
            while (true)
            {
				Debug.Print("result = PASS");
				Debug.Print("accuracy = null");
				Debug.Print("resultParameter1 = null");
				Debug.Print("resultParameter2 = null");
				Debug.Print("resultParameter3 = null");
				Debug.Print("resultParameter4 = null");
				Debug.Print("resultParameter5 = null"); 
            }
        }
    }
}
