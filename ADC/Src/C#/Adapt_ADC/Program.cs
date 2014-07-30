using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;
using Samraksh.eMote.Adapt;


namespace TestSuite
{
    public class Program
    {
		public const uint bufferSize = 1000;

        public const uint sampleTime = 1000;

        public static ushort[] sampleBuffer = new ushort[bufferSize];

        public static Samraksh.eMote.Adapt.AdcCallBack adcCallbackPtr;


        public static void AdcCallbackFn(long NativeTime)
        {               
        	Debug.Print("Callback: " + (NativeTime).ToString() + "  " + DateTime.Now.Ticks.ToString());                    
        }

        public static void Main()
        {
			int retVal;

            Parameters parameters = new Parameters();
			Debug.Print("Starting ADC calls");

			adcCallbackPtr = AdcCallbackFn;
            retVal = Samraksh.eMote.Adapt.AnalogInput.Initialize();
			Debug.Print("Initialize: " + retVal.ToString());
            retVal = Samraksh.eMote.Adapt.AnalogInput.ConfigureContinuousMode(sampleBuffer, Samraksh.eMote.Adapt.AdcChannel.ADC_Channel1 | Samraksh.eMote.Adapt.AdcChannel.ADC_Channel7, bufferSize, sampleTime, AdcCallbackFn);
			Debug.Print("configure cont: " + retVal.ToString());
			retVal = Samraksh.eMote.Adapt.AnalogInput.Uninitialize();
			Debug.Print("uninitialize: " + retVal.ToString());

			Debug.Print("Done with ADC calls");
            while (true)
            {
                System.Threading.Thread.Sleep(10000);
            }
        }
    }
}
