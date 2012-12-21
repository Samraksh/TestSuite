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
            while (true)
            {
                lcd.Initialize();
		lcd.Blink(Blink.OFF);
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
		lcd.Clear();
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_e, LCD.CHAR_f, LCD.CHAR_g, LCD.CHAR_h);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_i, LCD.CHAR_j, LCD.CHAR_k, LCD.CHAR_l);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_m, LCD.CHAR_n, LCD.CHAR_o, LCD.CHAR_p);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_q, LCD.CHAR_r, LCD.CHAR_s, LCD.CHAR_t);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_u, LCD.CHAR_v, LCD.CHAR_w, LCD.CHAR_x);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_y, LCD.CHAR_z, LCD.CHAR_NULL, LCD.CHAR_NULL);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_0, LCD.CHAR_1, LCD.CHAR_2, LCD.CHAR_3);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_4, LCD.CHAR_5, LCD.CHAR_6, LCD.CHAR_7);
                Thread.Sleep(3000);
		lcd.SetDP(false, false, false, false);
                lcd.Write(LCD.CHAR_4, LCD.CHAR_5, LCD.CHAR_6, LCD.CHAR_7);
                Thread.Sleep(3000);
		lcd.SetDP(false, false, false, true);
                lcd.Write(LCD.CHAR_4, LCD.CHAR_5, LCD.CHAR_6, LCD.CHAR_7);
                Thread.Sleep(3000);
		lcd.SetDP(false, false, true, true);
                Thread.Sleep(3000);
		lcd.SetDP(false, true, true, true);
                Thread.Sleep(3000);
		lcd.SetDP(true, true, true, true);
                Thread.Sleep(3000);
                lcd.Write(LCD.CHAR_8, LCD.CHAR_9, LCD.CHAR_NULL, LCD.CHAR_NULL);
                Thread.Sleep(3000);
		lcd.WriteN(1,LCD.CHAR_A);
                Thread.Sleep(3000);
		lcd.WriteN(2,LCD.CHAR_A);
                Thread.Sleep(3000);
		lcd.WriteN(3,LCD.CHAR_A);
                Thread.Sleep(3000);
		lcd.WriteN(4,LCD.CHAR_A);
                Thread.Sleep(3000);
		lcd.WriteRawBytes(0x11, 0x22, 0x33, 0x44);
                Thread.Sleep(3000);
		lcd.Blink(Blink.SLOW);
                Thread.Sleep(3000);
		lcd.Blink(Blink.MEDIUM);
                Thread.Sleep(3000);
		lcd.Blink(Blink.FAST);
                Thread.Sleep(3000);
                lcd.Uninitialize();
                Thread.Sleep(3000);
            }
        }
    }
}
