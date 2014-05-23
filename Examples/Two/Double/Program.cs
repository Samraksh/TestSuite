using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;
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
            while (true)
            {
                lcd.Write(LCD.CHAR_A, LCD.CHAR_B, LCD.CHAR_C, LCD.CHAR_D);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_E, LCD.CHAR_F, LCD.CHAR_G, LCD.CHAR_H);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_I, LCD.CHAR_J, LCD.CHAR_K, LCD.CHAR_L);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_M, LCD.CHAR_N, LCD.CHAR_O, LCD.CHAR_P);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_Q, LCD.CHAR_R, LCD.CHAR_S, LCD.CHAR_T);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_U, LCD.CHAR_V, LCD.CHAR_W, LCD.CHAR_X);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_Y, LCD.CHAR_Z, LCD.CHAR_NULL, LCD.CHAR_NULL);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_a, LCD.CHAR_b, LCD.CHAR_c, LCD.CHAR_d);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_e, LCD.CHAR_f, LCD.CHAR_g, LCD.CHAR_h);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_i, LCD.CHAR_j, LCD.CHAR_k, LCD.CHAR_l);
                Thread.Sleep(3000);
            }
        }
    }
}
