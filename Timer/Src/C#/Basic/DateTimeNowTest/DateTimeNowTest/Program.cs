using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Tests
{
    public class DateTimeNowTest
    {

        public const int numberOfIterations = 2000;

        public void DisplayStats(string result, string resultParameter1, string resultParameter2, string accuracy, string resultParameter3, string resultParameter4, string resultParameter5)
        {
            Debug.Print("result =  " + result + "\n");
            Debug.Print("accuracy =  " + accuracy + "\n");
            Debug.Print("resultParameter1 =  " + resultParameter1 + "\n");
            Debug.Print("resultParameter2 =  " + resultParameter2 + "\n");
            Debug.Print("resultParameter3 =  " + resultParameter3 + "\n");
            Debug.Print("resultParameter4 =  " + resultParameter4 + "\n");
            Debug.Print("resultParameter5 =  " + resultParameter5 + "\n");

            

        }

        public void Run()
        {
            int counter = 0;
            long time1 = 0;
            long time2 = 0;

            while (counter++ < numberOfIterations)
            {
                time1 = DateTime.Now.Ticks;
                System.Threading.Thread.Sleep(200);
                time2 = DateTime.Now.Ticks;

                if (time2 - time1 <= 0)
                {
                    DisplayStats("FAIL", "DateTime.Now.Ticks test failed", "Current time less than past time", "0", null, null, null);
                    return;
                }

            }

            DisplayStats("PASS", "DateTime.Now.Ticks test passed", null, "100", null, null, null);

        }

        public static void Main()
        {
            DateTimeNowTest test = new DateTimeNowTest();

            test.Run();
        }

    }
}
