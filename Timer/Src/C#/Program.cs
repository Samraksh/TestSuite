using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
		private static OutputPort testPort_PB8 = new OutputPort(GPIOPins.GPIO_PIN_PB8, true);
       
		
        public static void Main()
        {
		Parameters parameters = new Parameters();
		double readFrequency = parameters.frequency;
		int msThreadSleep = (int)( (1/readFrequency)/2 * 1000);

        	while(true)
			{
			testPort_PB8.Write(true);
			
			Thread.Sleep(msThreadSleep);

			testPort_PB8.Write(false);
		
			Thread.Sleep(msThreadSleep);
		}
        }

    }
}
