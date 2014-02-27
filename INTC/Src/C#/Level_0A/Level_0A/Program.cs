using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;

/* This is a simple program that measures the latency between timer in C# vs timer interrupt (TIM2) in native code. The way this works is as follows:
 * 1. Deploy this app into the mote. If it cannot be deployed for some reason, convert this to a s19 file and then write to flash. 
 * 2. Let the app run and capture output on a terminal. 
 * 3. The terminal displays values such as the number of times TIM2 is invoked, the avg, min and max delays. A sample output is shown below:
 *     a. ISR index 28; C 24300; D 0; maxD 0; minD 0 
 *     b. maxD and minD may not be visible since they are of type double 
 * 4. Use gdb and measure the minD for every invocation of TIM2. I got it as 4.2us.
 * 5. Find difference between the count value for every display of "a" from terminal. I got it as 4900 for 1000ms and 500 for 100ms.
 * 6. Multiplying 4900 with 4.2us gives 21.07ms and 500 * 4.2us is 2.15ms.
 * 7. TIM2 was not invoked for 10ms
*/

namespace Level_0A
{
    public class Program
    {
        public static OutputPort timerPort = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4, false);

        public static void Main()
        {
            while (true)
            {
                timerPort.Write(true);
                Debug.Print("a");
                System.Threading.Thread.Sleep(100);
                //System.Threading.Thread.Sleep(100);
                //System.Threading.Thread.Sleep(10);
                timerPort.Write(false);
            }
        }

    }
}
