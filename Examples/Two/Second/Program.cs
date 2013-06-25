using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
        public static void Main()
        {
            Parameters parameters = new Parameters();
            EmoteLCD lcd = new EmoteLCD();
			lcd.Initialize();
			lcd.Blink(Blink.OFF);
            while (true)
            {
                lcd.Write(LCD.CHAR_A, LCD.CHAR_A, LCD.CHAR_C, LCD.CHAR_D);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_E, LCD.CHAR_F, LCD.CHAR_G, LCD.CHAR_H);
                Thread.Sleep(3000);
                
            }
        }
    }
}
