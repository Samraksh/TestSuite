using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace TestSuite
{
    public class Program
    {
		public static bool state = false;
		private static OutputPort testPort = new OutputPort(EmotePins.GPIO_PIN_0, true);		
       
		
        public static void Main()
        {
			Timer tmr = new Timer(new TimerCallback(Callback), null, 0, 1000);								
			
			while (true)
            {
				Thread.Sleep(100);
            }	
           
        }
		private static void Callback(Object state)
        {
            testPort.Write(true);						
			testPort.Write(false);
        }	

    }
}
