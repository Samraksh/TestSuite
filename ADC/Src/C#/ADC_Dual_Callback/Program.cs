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
		private static OutputPort testPort_PB8 = new OutputPort((Cpu.Pin)24, true);
		public const uint bufferSize = 1000;

        public const uint sampleTime = 1000;

		private static readonly ushort[] Ibuffer = new ushort[bufferSize];
        private static readonly ushort[] Qbuffer = new ushort[bufferSize];
		
		private static int callBackCnt = 0;	

        public static ushort[] sampleBuffer = new ushort[bufferSize];

        public static Samraksh.eMote.DotNow.AdcCallBack adcCallbackPtr;


        public static void AdcCallbackFn(long NativeTime)
        {               
			testPort_PB8.Write(true);
            //Debug.Print((NativeTime/54000000).ToString());
			callBackCnt++;
            Debug.Print(callBackCnt.ToString() + " " + (NativeTime).ToString() + "  " + DateTime.Now.Ticks.ToString()); 
			System.Threading.Thread.Sleep(1200);			
			testPort_PB8.Write(false);
        }

        public static void Main()
        {
			Debug.EnableGCMessages(true);

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
				//testPort_PB8.Write(true);
                System.Threading.Thread.Sleep(4000);
				//testPort_PB8.Write(false);
				
            }
        }
    }
}
