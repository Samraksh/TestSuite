using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Tests
{

    public class ADCCBMTest
    {
        public static uint numberOfCallbacks = 0;

        public static UInt32 startTime = 0;
        public static UInt32 endTime = 0;

        public static bool callbackHappened = false;

        public static OutputPort timer = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);

        public static AnalogOutput dac;

        public const uint numberOfEvents = 100;

        public static bool dacValueChanged = false;

        public static int dacValue = 0;

        public static bool error = false;

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

        public static void dacValueChange(Object state)
        {

            dac.WriteRaw(dacValue);

            dacValue += 500;

            if (dacValue > 4000)
                dacValue = 500;

            dacValueChanged = true;

            Debug.Print("Current Dac value is : " + dacValue.ToString() + "\n");

        }

        public static void adcSamples(uint thresholdTime)
        {
            endTime = (UInt32)(DateTime.Now.Millisecond);
            numberOfCallbacks++;
            callbackHappened = true;
        }

        public static void Main()
        {
            uint numSamples = 2000;

            ushort[] sampleBuffer = new ushort[numSamples];

            

            uint eventId = 0;

            uint timeout = 0;

            Samraksh.SPOT.Hardware.EmoteDotNow.AdcCallBack adcSampleCallback;

            adcSampleCallback = adcSamples;

            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitializeADC();
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);

            dac = new AnalogOutput((Cpu.AnalogOutputChannel)0);

            //Timer dacValueChanger = new Timer(dacValueChange, null, 0, 2500);

            dacValue = 1500;

            dac.WriteRaw(dacValue);

            startTime = (UInt32) (DateTime.Now.Millisecond);

            timer.Write(true);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.ConfigureBatchMode(sampleBuffer, Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1, numSamples, 500, adcSampleCallback);

            // Check to make sure callback happened at least once 
            while (!callbackHappened) ;
            timer.Write(false);

            Debug.Print("Time elapsed : " + (endTime - startTime).ToString() + "\n");

            for (UInt16 i = 0; i < numSamples; i++)
            {

                 if ((sampleBuffer[i] - dacValue) > 300)
                 {

                     
                      error = true;

                      //PrintStats(false, "ADC Batch Test failed", "Samples contain values different from set dac level", "", "", "", 0);
                      //break;
                  }
            }


            if (!error)
                PrintStats(true, "ADC Batch Test Success", "", null, null, null, 0);
            else
                PrintStats(false, "ADC Batch Test Failed", "", "", "", "", 0);

        }

    }
}
