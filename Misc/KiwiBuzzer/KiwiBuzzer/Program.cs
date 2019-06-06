using System;
using System.Threading;
using Microsoft.SPOT;
using Samraksh.eMote.SensorBoard;

namespace KiwiBuzzer
{
    public class Program
    {
        Buzzer buzz = new Buzzer();
        public static void Main()
        {
            while (true)
            {
                Buzzer.On();
                Thread.Sleep(2000);
                Buzzer.Off();
                Thread.Sleep(4000);
            }
        }
    }
}
