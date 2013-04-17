using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{
    public class InputPortTestMaster
    {

        private static OutputPort testPort_PA0 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN3, false);
        private static OutputPort testPort_PA1 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN4, false);
        private static OutputPort testPort_PA2 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN5, false);
        private static OutputPort testPort_PA3 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN6, false);
        private static OutputPort testPort_PA4 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN7, false);
        private static OutputPort testPort_PA8 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN8, false);
        private static OutputPort testPort_PB6 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN9, false);
        private static OutputPort testPort_PB7 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN10, false);
        private static OutputPort testPort_PB8 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);
        private static OutputPort testPort_PB9 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false);
        private static OutputPort testPort_PB13 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN3, false);
        private static OutputPort testPort_PB14 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4, false);
        private static OutputPort testPort_PB15 = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN5, false);
        
        public static void Main()
        {
            Parameters parameters = new Parameters();
            double readFrequency = parameters.frequency;
            int msThreadSleep = (int)((1 / readFrequency) / 2 * 1000);

            while (true)
            {
                testPort_PA0.Write(true);
                testPort_PA1.Write(true);
                testPort_PA2.Write(true);
                testPort_PA3.Write(true);
                testPort_PA4.Write(true);
                testPort_PA8.Write(true);
                testPort_PB6.Write(true);
                testPort_PB7.Write(true);
                testPort_PB8.Write(true);
                testPort_PB9.Write(true);
                testPort_PB13.Write(true);
                testPort_PB14.Write(true);
                testPort_PB15.Write(true);

               
                System.Threading.Thread.Sleep(msThreadSleep);

                testPort_PA0.Write(false);
                testPort_PA1.Write(false);
                testPort_PA2.Write(false);
                testPort_PA3.Write(false);
                testPort_PA4.Write(false);
                testPort_PA8.Write(false);
                testPort_PB6.Write(false);
                testPort_PB7.Write(false);
                testPort_PB8.Write(false);
                testPort_PB9.Write(false);
                testPort_PB13.Write(false);
                testPort_PB14.Write(false);
                testPort_PB15.Write(false);
            

                System.Threading.Thread.Sleep(msThreadSleep);
            }
        }

    }
}
