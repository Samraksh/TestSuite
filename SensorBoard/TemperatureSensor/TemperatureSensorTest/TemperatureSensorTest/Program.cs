using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.SensorBoard;
using System.Threading;

namespace Samraksh.SPOT.Tests
{
    public class TemperatureSensorTest
    {
        public static void Main()
        {
            Cpu.Pin bus = (Cpu.Pin)Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4;

            TemperatureSensor ts = new TemperatureSensor(bus);

            while (true)
            {
                Thread.Sleep(500);
            }

        }

    }
}
