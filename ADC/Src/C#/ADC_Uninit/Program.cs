using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;
using Samraksh.eMote.DotNow;


namespace TestSuite
{
    public class Program
    {
		public const uint bufferSize = 1000;

        public const uint sampleTime = 1000;

        public static ushort[] sampleBuffer = new ushort[bufferSize];

        public static Samraksh.eMote.DotNow.AdcCallBack adcCallbackPtr;

		public static bool reInit = false;


        public static void AdcCallbackFn(long NativeTime)
        {               
			if (reInit == true)
			{
            	Debug.Print((NativeTime).ToString() + "  " + DateTime.Now.Ticks.ToString());                    
			}
        }

        public static void Main()
        {
			Debug.EnableGCMessages(false);

            Parameters parameters = new Parameters();
            EmoteLCD lcd = new EmoteLCD();
            lcd.Initialize();
			lcd.Blink(Blink.OFF);
            lcd.Write(LCD.CHAR_A, LCD.CHAR_E, LCD.CHAR_C, LCD.CHAR_D);

			int ret;
			adcCallbackPtr = AdcCallbackFn;
            Samraksh.eMote.DotNow.AnalogInput.InitializeADC();
            Samraksh.eMote.DotNow.AnalogInput.InitChannel(Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1);
            ret = Samraksh.eMote.DotNow.AnalogInput.ConfigureContinuousMode(sampleBuffer, Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1, bufferSize, sampleTime, AdcCallbackFn);
			System.Threading.Thread.Sleep(4000);
            Samraksh.eMote.DotNow.AnalogInput.UninitializeADC();
			System.Threading.Thread.Sleep(2000);
			adcCallbackPtr = AdcCallbackFn;
            Samraksh.eMote.DotNow.AnalogInput.InitializeADC();
            Samraksh.eMote.DotNow.AnalogInput.InitChannel(Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1);
            Samraksh.eMote.DotNow.AnalogInput.ConfigureContinuousMode(sampleBuffer, Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1, bufferSize, sampleTime, AdcCallbackFn);
			reInit = true;

            while (true)
            {
                //System.Threading.Thread.Sleep(10000);
            }
        }
    }
}
