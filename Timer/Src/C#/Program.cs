using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
#if (HARDWARE_EMOTE)
	private static OutputPort testPort_PA0 = new OutputPort(GPIOPins.GPIO_PIN_PA0, true);
	private static OutputPort testPort_PA1 = new OutputPort(GPIOPins.GPIO_PIN_PA1, true);
	private static OutputPort testPort_PA2 = new OutputPort(GPIOPins.GPIO_PIN_PA2, true);
	private static OutputPort testPort_PA3 = new OutputPort(GPIOPins.GPIO_PIN_PA3, true);
	private static OutputPort testPort_PA4 = new OutputPort(GPIOPins.GPIO_PIN_PA4, true);
	private static OutputPort testPort_PA5 = new OutputPort(GPIOPins.GPIO_PIN_PA5, true);
	private static OutputPort testPort_PA6 = new OutputPort(GPIOPins.GPIO_PIN_PA6, true);
	private static OutputPort testPort_PA7 = new OutputPort(GPIOPins.GPIO_PIN_PA7, true);
	private static OutputPort testPort_PA8 = new OutputPort(GPIOPins.GPIO_PIN_PA8, true);
	private static OutputPort testPort_PA9 = new OutputPort(GPIOPins.GPIO_PIN_PA9, true);
	private static OutputPort testPort_PA10 = new OutputPort(GPIOPins.GPIO_PIN_PA10, true);

	private static OutputPort testPort_PB0 = new OutputPort(GPIOPins.GPIO_PIN_PB0, true);
	private static OutputPort testPort_PB1 = new OutputPort(GPIOPins.GPIO_PIN_PB1, true);
	private static OutputPort testPort_PB2 = new OutputPort(GPIOPins.GPIO_PIN_PB2, true);
	private static OutputPort testPort_PB3 = new OutputPort(GPIOPins.GPIO_PIN_PB3, true);
	private static OutputPort testPort_PB4 = new OutputPort(GPIOPins.GPIO_PIN_PB4, true);
	private static OutputPort testPort_PB5 = new OutputPort(GPIOPins.GPIO_PIN_PB5, true);
	private static OutputPort testPort_PB6 = new OutputPort(GPIOPins.GPIO_PIN_PB6, true);
	private static OutputPort testPort_PB7 = new OutputPort(GPIOPins.GPIO_PIN_PB7, true);
	private static OutputPort testPort_PB8 = new OutputPort(GPIOPins.GPIO_PIN_PB8, true);
	private static OutputPort testPort_PB9 = new OutputPort(GPIOPins.GPIO_PIN_PB9, true);
	private static OutputPort testPort_PB10 = new OutputPort(GPIOPins.GPIO_PIN_PB10, true);
	private static OutputPort testPort_PB11 = new OutputPort(GPIOPins.GPIO_PIN_PB11, true);
	private static OutputPort testPort_PB12 = new OutputPort(GPIOPins.GPIO_PIN_PB12, true);
	private static OutputPort testPort_PB13 = new OutputPort(GPIOPins.GPIO_PIN_PB13, true);
	private static OutputPort testPort_PB14 = new OutputPort(GPIOPins.GPIO_PIN_PB14, true);
	private static OutputPort testPort_PB15 = new OutputPort(GPIOPins.GPIO_PIN_PB15, true);

	private static OutputPort testPort_PC6 = new OutputPort(GPIOPins.GPIO_PIN_PC6, true);
#elif (HARDWARE_ADAPT)
	//private static OutputPort testPort_55 = new OutputPort((Cpu.Pin)55, true);
	private static OutputPort testPort_PA0 = new OutputPort(GPIOPins.GPIO_PIN_PA0, true);
	private static OutputPort testPort_PA1 = new OutputPort(GPIOPins.GPIO_PIN_PA1, true);
	private static OutputPort testPort_PA2 = new OutputPort(GPIOPins.GPIO_PIN_PA2, true);
	private static OutputPort testPort_PA3 = new OutputPort(GPIOPins.GPIO_PIN_PA3, true);
	private static OutputPort testPort_PA4 = new OutputPort(GPIOPins.GPIO_PIN_PA4, true);
	private static OutputPort testPort_PA5 = new OutputPort(GPIOPins.GPIO_PIN_PA5, true);
	private static OutputPort testPort_PA6 = new OutputPort(GPIOPins.GPIO_PIN_PA6, true);
	private static OutputPort testPort_PA7 = new OutputPort(GPIOPins.GPIO_PIN_PA7, true);
	private static OutputPort testPort_PA8 = new OutputPort(GPIOPins.GPIO_PIN_PA8, true);
	private static OutputPort testPort_PA9 = new OutputPort(GPIOPins.GPIO_PIN_PA9, true);

#endif	
        public static void Main()
        {
		Parameters parameters = new Parameters();
		double readFrequency = parameters.frequency;
		int msThreadSleep = (int)( (1/readFrequency)/2 * 1000);

        	while(true)
			{
#if (HARDWARE_EMOTE)				
			testPort_PA0.Write(true);
			testPort_PA1.Write(true);
			testPort_PA2.Write(true);
			testPort_PA3.Write(true);
			testPort_PA4.Write(true);
			testPort_PA8.Write(true);
			testPort_PB6.Write(true);
			testPort_PB7.Write(true);
			testPort_PB8.Write(true);
			testPort_PB9.Write(true);
			testPort_PB13.Write(true);
			testPort_PB14.Write(true);
			testPort_PB15.Write(true);

			testPort_PB13.Write(true);
			testPort_PB14.Write(true);
			testPort_PB15.Write(true);
			testPort_PA2.Write(true);
			testPort_PA3.Write(true);
			testPort_PB6.Write(true);
			testPort_PB7.Write(true);

			testPort_PA9.Write(true);
			testPort_PA10.Write(true);
			testPort_PC6.Write(true);
			Thread.Sleep(msThreadSleep);

			testPort_PA0.Write(false);
			testPort_PA1.Write(false);
			testPort_PA2.Write(false);
			testPort_PA3.Write(false);
			testPort_PA4.Write(false);
			testPort_PA8.Write(false);
			testPort_PB6.Write(false);
			testPort_PB7.Write(false);
			testPort_PB8.Write(false);
			testPort_PB9.Write(false);
			testPort_PB13.Write(false);
			testPort_PB14.Write(false);
			testPort_PB15.Write(false);

			testPort_PB13.Write(false);
			testPort_PB14.Write(false);
			testPort_PB15.Write(false);
			testPort_PA2.Write(false);
			testPort_PA3.Write(false);
			testPort_PB6.Write(false);
			testPort_PB7.Write(false);

			testPort_PA9.Write(false);
			testPort_PA10.Write(false);
			testPort_PC6.Write(false);
			Thread.Sleep(msThreadSleep);
#elif (HARDWARE_ADAPT)
			testPort_PA0.Write(true);
			testPort_PA1.Write(true);
			//testPort_PA2.Write(true);
			testPort_PA3.Write(true);
			testPort_PA4.Write(true);
			testPort_PA5.Write(true);
			testPort_PA6.Write(true);
			testPort_PA7.Write(true);
			testPort_PA8.Write(true);
			testPort_PA9.Write(true);
			Thread.Sleep(msThreadSleep);

			testPort_PA0.Write(false);
			testPort_PA1.Write(false);
			//testPort_PA2.Write(false);
			testPort_PA3.Write(false);
			testPort_PA4.Write(false);
			testPort_PA5.Write(false);
			testPort_PA6.Write(false);
			testPort_PA7.Write(false);
			testPort_PA8.Write(false);
			testPort_PA9.Write(false);
			Thread.Sleep(msThreadSleep);
#endif			
		}
        }

    }
}
