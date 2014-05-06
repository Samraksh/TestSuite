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

        public static ushort[] sampleBuffer1 = new ushort[bufferSize];
		public static ushort[] sampleBuffer2 = new ushort[bufferSize];
        public static Samraksh.eMote.DotNow.AdcCallBack adcCallbackPtr;


        public static void AdcCallbackFn(long NativeTime)
        {               
            Debug.Print((NativeTime).ToString() + "  " + DateTime.Now.Ticks.ToString()); 
        }

        public static void Main()
        {
			Debug.EnableGCMessages(false);

            Parameters parameters = new Parameters();
            EmoteLCD lcd = new EmoteLCD();
            lcd.Initialize();
			lcd.Blink(Blink.OFF);
            lcd.Write(LCD.CHAR_A, LCD.CHAR_E, LCD.CHAR_C, LCD.CHAR_D);

			adcCallbackPtr = AdcCallbackFn;
            Samraksh.eMote.DotNow.AnalogInput.InitializeADC();
            Samraksh.eMote.DotNow.AnalogInput.ConfigureContinuousModeDualChannel(sampleBuffer1, sampleBuffer2, bufferSize, sampleTime, AdcCallbackFn);

            while (true)
            {
                System.Threading.Thread.Sleep(10000);
            }
        }
    }
}
