using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{
    public class SleepTest
    {

        public OutputPort port1 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);

        public const int numberOfRuns = 2000;

        public void Run()
        {
            int counter = 0;

            while (counter++ < numberOfRuns)
            {
                port1.Write(true);
                System.Threading.Thread.Sleep(200);
                port1.Write(false);
            }

        }


        public static void Main()
        {
            SleepTest test = new SleepTest();

            test.Run();

        }

    }
}
