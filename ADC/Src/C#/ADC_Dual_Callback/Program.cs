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
		private static OutputPort testPort_PA0 = new OutputPort((Cpu.Pin)0, true);
		public const uint bufferSize = 100;

        public const uint sampleTime = 250;

		private static readonly ushort[] Ibuffer = new ushort[bufferSize];
        private static readonly ushort[] Qbuffer = new ushort[bufferSize];
		
		private static int callBackCnt = 0;	

        public static ushort[] sampleBuffer = new ushort[bufferSize];

        public static Samraksh.eMote.DotNow.AdcCallBack adcCallbackPtr;


        public static void AdcCallbackFn(long NativeTime)
        {               
            //Debug.Print((NativeTime/54000000).ToString());
			callBackCnt++;
            Debug.Print(callBackCnt.ToString() + " " + (NativeTime).ToString() + "  " + DateTime.Now.Ticks.ToString());                    
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
            Samraksh.eMote.DotNow.AnalogInput.ConfigureContinuousModeDualChannel(Ibuffer, Qbuffer, bufferSize, sampleTime, AdcCallbackFn);
            //Samraksh.eMote.DotNow.AnalogInput.InitChannel(Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1);
            //Samraksh.eMote.DotNow.AnalogInput.ConfigureContinuousMode(sampleBuffer, Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1, bufferSize, sampleTime, AdcCallbackFn);

            while (true)
            {
				testPort_PA0.Write(true);
                System.Threading.Thread.Sleep(10000);
				testPort_PA0.Write(false);
            }
        }
    }
}
