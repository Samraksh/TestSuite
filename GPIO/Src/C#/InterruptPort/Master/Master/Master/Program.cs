using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Master
{
    

    public class Program
    {

        public static OutputPort port1 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);

        public static void Callback(Object state)
        {
            port1.Write(true);
            Thread.Sleep(200);
            port1.Write(false);
        }

        public static void Main()
        {

            Timer mytimer = new Timer(Callback, null, 200, 600);

            while (true)
            {
                Thread.Sleep(500);
            }
        }

    }
}
