using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace GPIO
{
    public class Program
    {
		private static OutputPort testPort_55 = new OutputPort((Cpu.Pin)55, true);
        public static void Main()
        {
			Debug.Print("Main entered\r\n");
            while (true){
				testPort_55.Write(true);
				testPort_55.Write(false);
			}
        }
    }
}
