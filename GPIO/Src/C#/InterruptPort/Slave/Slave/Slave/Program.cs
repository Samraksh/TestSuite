using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Slave
{
    public class Program
    {
        public static InterruptPort port1 = new InterruptPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN7, false, Port.ResistorMode.Disabled, Port.InterruptMode.InterruptEdgeHigh);
        public static OutputPort port2 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);


        public static void Main()
        {
            port1.OnInterrupt += port1_OnInterrupt;

            port1.EnableInterrupt();

            while (true)
            {
                Thread.Sleep(500);
            }

        }

        static void port1_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            port2.Write(true);
            port2.Write(false);
        }

    }
}
