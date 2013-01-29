using System;
using System.Threading;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Testing
{
    public class ADCTest
    {
        public static ADCTest adctest;
        private Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput _adc;

        public ADCTest()
        {
            _adc = new Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput();
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.ADC_Channel1);
        }

        public void Run()
        {
            while (true)
            {
                double adcVal = Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Read(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.ADC_Channel1);
                Debug.Print(adcVal.ToString());
                Thread.Sleep(100);
            }
        }

        public static void Main()
        {
            
            try
            {
                adctest = new ADCTest();
            }
            catch (Exception e)
            {
                Debug.Print("Test Instance creation throwing exception");
            }
            try
            {
                adctest.Run();
            }
            catch (Exception e)
            {
                Debug.Print("Test run threw an exception");
            }
        }

    }
}
