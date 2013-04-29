using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{
    public class InputPortTestSlave
    {
        public static Int16 data;
        private static InputPort testPort_PA0 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN3, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PA1 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN4, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PA2 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN5, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PA3 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN6, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PA4 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN7, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PA8 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN8, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB6 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN9, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB7 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN10, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB8 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB9 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB13 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN3, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB14 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4, false, Port.ResistorMode.PullDown);
        private static InputPort testPort_PB15 = new InputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN5, false, Port.ResistorMode.PullDown);

        public static void Main()
        {
            testPort_PA0.OnInterrupt += testPort_PA0_OnInterrupt;
            testPort_PA1.OnInterrupt += testPort_PA1_OnInterrupt;
            testPort_PA2.OnInterrupt += testPort_PA2_OnInterrupt;
            testPort_PA3.OnInterrupt += testPort_PA3_OnInterrupt;
            testPort_PA4.OnInterrupt += testPort_PA4_OnInterrupt;
            testPort_PA8.OnInterrupt += testPort_PA8_OnInterrupt;
            testPort_PB6.OnInterrupt += testPort_PB6_OnInterrupt;
            testPort_PB7.OnInterrupt += testPort_PB7_OnInterrupt;
            testPort_PB8.OnInterrupt += testPort_PB8_OnInterrupt;
            testPort_PB9.OnInterrupt += testPort_PB9_OnInterrupt;
            testPort_PB13.OnInterrupt += testPort_PB13_OnInterrupt;
            testPort_PB14.OnInterrupt += testPort_PB14_OnInterrupt;
            testPort_PB15.OnInterrupt += testPort_PB15_OnInterrupt;

            testPort_PA0.EnableInterrupt();
            testPort_PA1.EnableInterrupt();
            testPort_PA2.EnableInterrupt();
            testPort_PA3.EnableInterrupt();
            testPort_PA4.EnableInterrupt();
            testPort_PA8.EnableInterrupt();
            testPort_PB6.EnableInterrupt();
            testPort_PB7.EnableInterrupt();
            testPort_PB8.EnableInterrupt();
            testPort_PB9.EnableInterrupt();
            testPort_PB13.EnableInterrupt();
            testPort_PB14.EnableInterrupt();
            testPort_PB15.EnableInterrupt();

            
        }

        static void testPort_PB9_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 9);
            
        }

        static void testPort_PB8_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 8);
            
        }

        static void testPort_PB7_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 7);
            
        }

        static void testPort_PB6_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 6);
            
        }

        static void testPort_PB15_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            Debug.Print("The Pin out value is " + data.ToString());
            data = 0;
        }

        static void testPort_PB14_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 14);
            
        }

        static void testPort_PB13_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 13);
            
        }

        static void testPort_PA8_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 8);
        }

        static void testPort_PA4_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 4);
        }

        static void testPort_PA3_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 3);
        }

        static void testPort_PA2_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 2);
        }

        static void testPort_PA1_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 1);
        }

        static void testPort_PA0_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            data |= (1 << 0);
        }

    }
}
