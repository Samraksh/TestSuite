using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
		public static void DisplayStats(bool result, string resultParameter1, string resultParameter2, int accuracy)
        {
			while (true){
				Thread.Sleep(1000);
            	if (result)
            	{
            	    Debug.Print("result=PASS\n");
            	}
            	else
            	{
            	    Debug.Print("result=FAIL\n");
            	}
            	Debug.Print("accuracy=" + accuracy.ToString());
            	Debug.Print("resultParameter1=" + resultParameter1);
            	Debug.Print("resultParameter2=" + resultParameter2);
            	Debug.Print("resultParameter3=null");
            	Debug.Print("resultParameter4=null");
            	Debug.Print("resultParameter5=null");
			}
        }

		static void testPort_PB15_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            Debug.Print("The Pin out value is " + data1.ToString() + " " + data2.ToString());
        }

#if (HARDWARE_EMOTE)
	private static OutputPort testPort_PA0 = new OutputPort(GPIOPins.GPIO_PIN_PA0, true);
	private static InputPort testPort_PB15 = new InputPort(GPIOPins.GPIO_PIN_PB15, false, Port.ResistorMode.Disabled, Port.InterruptMode.InterruptEdgeHigh);
	
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
			int testCnt = 0;
			testPort_PB15.OnInterrupt += testPort_PB15_OnInterrupt;
			testPort_PB15.EnableInterrupt();

        	while(testCnt<100)
			{
				testCnt++;
#if (HARDWARE_EMOTE)				
				testPort_PA0.Write(true);
				Thread.Sleep(1000);
				testPort_PA0.Write(false);
				Thread.Sleep(1000);
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
				DisplayStats(true, "GPIO input test passed", null, 0);
        }

    }
}
