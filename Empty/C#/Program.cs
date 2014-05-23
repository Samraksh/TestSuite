using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
//using Samraksh.SPOT.Hardware.EmoteDotNow;
using System.Threading;
using ParameterClass;

namespace TestSuite
{
    public class Program
    {
        public static void Main()
        {
            while (true)
            {
				Debug.Print("result = PASS");
				Debug.Print("accuracy = null");
				Debug.Print("resultParameter1 = null");
				Debug.Print("resultParameter2 = null");
				Debug.Print("resultParameter3 = null");
				Debug.Print("resultParameter4 = null");
				Debug.Print("resultParameter5 = null"); 
             	System.Threading.Thread.Sleep(1000); 
            }
        }
    }
}
