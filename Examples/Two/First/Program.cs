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
                lcd.Write(LCD.CHAR_m, LCD.CHAR_n, LCD.CHAR_o, LCD.CHAR_p);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_q, LCD.CHAR_r, LCD.CHAR_s, LCD.CHAR_t);
                Thread.Sleep(3000);
            }
        }
    }
}
