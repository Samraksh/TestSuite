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

        	while(true)
			{
			}
        }
    }
}
