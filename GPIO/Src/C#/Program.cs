using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
	private static OutputPort testPort = new OutputPort(EmotePins.GPIO_PIN_0, true);
	
        public static void Main()
        {
		Parameters parameters = new Parameters();
		double readFrequency = parameters.frequency;
		int msThreadSleep = (int)( (1/readFrequency)/2 * 1000);

        	while(true)
		{
			testPort.Write(true);
			Thread.Sleep(msThreadSleep);
			testPort.Write(false);
			Thread.Sleep(msThreadSleep);
		}
        }

    }
}
