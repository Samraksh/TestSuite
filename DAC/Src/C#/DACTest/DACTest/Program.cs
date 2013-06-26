using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Samraksh.SPOT.Tests
{
    public class DACTest
    {
        public static AnalogOutput dac = new AnalogOutput((Cpu.AnalogOutputChannel)0);

        public static void Main()
        {
            int dacLevel = 0;

            while (true)
            {

                Debug.Print("The current dac level : " + dacLevel.ToString() + "\n");

                dac.WriteRaw(dacLevel);

                dacLevel += 500;

                

                if (dacLevel > 4000)
                    dacLevel = 0;

                Thread.Sleep(5000);
            }
        }

    }
}
