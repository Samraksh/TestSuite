using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
        public static void Main()
        {
			EmoteLCD lcd = new EmoteLCD();
			lcd.Initialize();
            while (true)
            {
                
            }
        }
    }
}
