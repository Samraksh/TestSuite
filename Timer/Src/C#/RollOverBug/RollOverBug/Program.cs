using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{
    public class RollOverBug
    {

        public static OutputPort errPort = new OutputPort((Cpu.Pin)1, false);
        public static OutputPort alivePort = new OutputPort((Cpu.Pin)24 , true);
        public static bool state = false;

        public static void Main()
        {
            long currTime = 0;
            long prevTime = 0;

            while (true)
            {
                currTime = DateTime.Now.Ticks;

                if (currTime < prevTime)
                {
                    errPort.Write(true);
                    errPort.Write(false);
                }
                else
                {
                    alivePort.Write(true);
                    alivePort.Write(false);
                }

                prevTime = currTime;

            }
        }

    }
}
