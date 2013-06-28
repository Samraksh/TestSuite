using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using System.Threading;
using Microsoft.SPOT.Hardware;

namespace ADCCCMTest
{
    public class Program
    {

        public static Timer dacChangeTimer;

        public static bool samplesReady = false;

        public static int dacLevel = 0;

        public static void adcCallback(long threshold)
        {
            Debug.Print("The threshold value is : " + threshold.ToString() + "\n");
            Debug.Print("The current time in ticks is : " + DateTime.Now.Ticks.ToString() + "\n");

            samplesReady = true;
        }

        public static void Main()
        {
            uint numberOfSamples = 1000;

            uint samplingTime = 500;

            ushort[] sampleBuff = new ushort[numberOfSamples];

            uint counter = 0;

            bool error = false;

            AnalogOutput dac = new AnalogOutput((Cpu.AnalogOutputChannel)0);

            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitializeADC();

            Samraksh.SPOT.Hardware.EmoteDotNow.AdcCallBack adcCallbackdelegate = adcCallback;

            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);

            //Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.ConfigureContinuousMode(sampleBuff, Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1, numberOfSamples, samplingTime, adcCallbackdelegate);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.ConfigureContinuousModeWithThresholding(sampleBuff, Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1, numberOfSamples, samplingTime, 2000,  adcCallbackdelegate);

            while (counter++ < 1000)
            {
                Debug.Print("Waiting for Callback ");

                while (!samplesReady) ;

                Debug.Print("Callback recieved, samples ready");

                samplesReady = false;

                
                for (UInt16 i = 0; i < numberOfSamples; i++)
                {
                    if ((sampleBuff[i] - dacLevel) > 300)
                    {
                        //Debug.Print("Error Detected, samples too far apart, sampleBuffer[ " + i.ToString() + " ] = " + sampleBuff[i].ToString() + ", dacLevel " + dacLevel.ToString());
                        error = true;
                    }
                        
                }

                dacLevel += 500;

                if (dacLevel > 4000)
                    dacLevel = 0;

                dac.WriteRaw(dacLevel);


            }

            
        }

    }
}
