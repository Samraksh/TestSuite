using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Samraksh.SPOT.Tests
{
    public class ADCTest
    {
        public static Microsoft.SPOT.Hardware.AnalogOutput dac;

        public const int numberOfEvents = 5000;

        public static int dacLevel = 0;

        public static OutputPort alive = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);
        public static OutputPort complete = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false);

        public static bool errorDetected = false;

        public static void PrintStats(bool result, string resultParameter1, string resultParameter2, string resultParameter3, string resultParameter4, string resultParameter5, int accuracy)
        {
            if (result)
                Debug.Print("\r\nresult=PASS\r\n");
            else
                Debug.Print("\r\nresult=FAIL\r\n");

            Debug.Print("\r\nresultParamter1= " + resultParameter1 + "\r\n");
            Debug.Print("\r\nresultParamter2= " + resultParameter2 + "\r\n");
            Debug.Print("\r\nresultParamter3= " + resultParameter3 + "\r\n");
            Debug.Print("\r\nresultParamter4= " + resultParameter4 + "\r\n");
            Debug.Print("\r\nresultParamter5= " + resultParameter5 + "\r\n");

        }

        public static void adcChanger(Object state)
        {
            alive.Write(true);

            dac.WriteRaw(dacLevel);      

            Thread.Sleep(200);

            int adcLevel = (int) Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Read(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);

            if ((dacLevel - adcLevel) > 300)
            {
                Debug.Print("Error : DAC Level = " + dacLevel.ToString() + ", ADC Level = " + adcLevel.ToString());
                errorDetected = true;
            }

            dacLevel += 500;

            if (dacLevel > 4000)
                dacLevel = 0;



            alive.Write(false);

        }

        public static void Main()
        {
            int eventid = 0;

            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitializeADC();

            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);

            dac = new AnalogOutput((Cpu.AnalogOutputChannel)0);

            Timer adcChangeTimer = new Timer(adcChanger, null, 0, 500);

            while (eventid++ < numberOfEvents)
            {
                Thread.Sleep(500);
            }

            if (errorDetected)
            {
                PrintStats(false, "The ADC level 0 test failed", "The difference between adc and dac is too large", "", "", "", 0);
            }
            else
                PrintStats(true, "The ADC level 0 test passed", "", "", "", "", 0);

            complete.Write(true);

        }

    }
}
