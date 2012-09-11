using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace TestSuite
{
    public class Program
    {
		private static OutputPort testPort = new OutputPort(EmotePins.GPIO_PIN_0, true);
	
        public static void Main()
        {
           while(true)
		   {
				testPort.Write(true);
				Thread.Sleep(1000);
				testPort.Write(false);
		   }
        }

    }
}
