using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using System.Threading;
using ParameterClass;
using Samraksh.SPOT.Hardware;


namespace TestSuite
{
    public class Program
    {
		private static OutputPort testPort_PA0 = new OutputPort((Cpu.Pin)0, true);
		public const uint bufferSize = 1000;

        public const uint sampleTime = 1000;

        public static ushort[] sampleBuffer = new ushort[bufferSize];

        public static Samraksh.SPOT.Hardware.EmoteDotNow.AdcCallBack adcCallbackPtr;


        public static void AdcCallbackFn(long NativeTime)
        {               
            //Debug.Print((NativeTime/54000000).ToString());
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
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitializeADC();
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.ConfigureContinuousMode(sampleBuffer, Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1, bufferSize, sampleTime, AdcCallbackFn);

            while (true)
            {
				testPort_PA0.Write(true);
                System.Threading.Thread.Sleep(10000);
				testPort_PA0.Write(false);
            }
        }
    }
}
