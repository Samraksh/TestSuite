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
                lcd.Write(LCD.CHAR_Q, LCD.CHAR_R, LCD.CHAR_S, LCD.CHAR_T);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_U, LCD.CHAR_V, LCD.CHAR_W, LCD.CHAR_X);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_Y, LCD.CHAR_Z, LCD.CHAR_NULL, LCD.CHAR_NULL);
                Thread.Sleep(3000);
            }
        }
    }
}
