//#define ENABLE_DEBUG

using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.Threading;


namespace GPIOAnalysis
{
    class Program
    {
#if ENABLE_DEBUG
        const string fileParameters = "D:\\AnanthAtSamraksh\\TestSuite\\GPIO\\Src\\Native\\Level_0A\\Parameters.h";
        const string fileName = "D:\\AnanthAtSamraksh\\TestSuite\\GPIO\\Src\\Native\\Level_0A\\testTemp-Copy\\testData.csv";
        const string fileOut = "D:\\AnanthAtSamraksh\\TestSuite\\GPIO\\Src\\Native\\Level_0A\\testTemp-Copy\\results.txt";
#else
        const string fileParameters = @"Parameters.h";
        const string fileName = @"testTemp\testData.csv";
        const string fileOut = @"testTemp\results.txt";
#endif

        static void Main(string[] args)
        {
            //int lineCount = File.ReadLines(fileName).Count();
            
            int[] callbackTimeValueTemp = new int[2];
            List<int> callbackTimeValue = new List<int>();
            bool testResult = false;
            char[] trimChars = new char[] { ' ', ']' };
            int gpioToggleIndex = 0;
            int sampleFrequency = 0, frequency = 0, upperAllowedFrequency = 0, lowerAllowedFrequency = 0, expectedFrequency = 0;

            try
            {
                using (StreamReader reader = new StreamReader(fileParameters))
                {
                    string line;
                    string[] timeString;
                    line = reader.ReadLine();

                    while (line != null)
                    {
                        line = line.Replace("   ", " ");
                        line = line.Replace("  ", " ");
                        timeString = line.Split(' ');
                        if ( !(String.Equals(timeString[0], "")) )
                        {
                            if (String.Equals(timeString[1], "sampleFrequency"))
                                sampleFrequency = int.Parse(timeString[2]);
                            else if (String.Equals(timeString[1], "frequency"))
                                frequency = int.Parse(timeString[2]);
                            else if (String.Equals(timeString[1], "upperAllowedFrequency"))
                                upperAllowedFrequency = int.Parse(timeString[2]);
                            else if (String.Equals(timeString[1], "lowerAllowedFrequency"))
                                lowerAllowedFrequency = int.Parse(timeString[2]);
                            else if (String.Equals(timeString[1], "expectedFrequency"))
                                expectedFrequency = int.Parse(timeString[2]);
                        }
                        line = reader.ReadLine();
                    }
                    reader.Close();
                }

                using (StreamReader reader = new StreamReader(fileName))
                {
                    string line;
                    string[] timeString;

                    line = reader.ReadLine();

                    //Consider only lines that have gpio toggle state as 1. Read all such lines in file. "gpioToggleIndex" keeps track of such lines
                    //Find difference between times of such successive values and store in array. 
                    //      Successive time values are stored in temporary array "callbackTimeValueTemp". Differences are stored in "callbackTimeValue".
                    //Finally, find their average and check if it is within the allowable limits.
                    
                    while ((line != null))
                    {
                        //System.Diagnostics.Debug.WriteLine(line);

                        line = line.Replace("   ", " ");
                        line = line.Replace("  ", " ");
                        timeString = line.Split(',');
                        if (int.Parse(timeString[1]) == 1)
                        {
                            if (gpioToggleIndex == 0)
                                callbackTimeValueTemp[0] = int.Parse(timeString[0]);
                            else
                                callbackTimeValueTemp[1] = int.Parse(timeString[0]);

                            if (gpioToggleIndex > 0)
                            {
                                callbackTimeValue.Add(callbackTimeValueTemp[1] - callbackTimeValueTemp[0]);
                                callbackTimeValueTemp[0] = callbackTimeValueTemp[1];
                            }
                            ++gpioToggleIndex;
                        }
                        line = reader.ReadLine();
                    }
                    reader.Close();
                }

                //callbackTimeValue = callbackTimeValue.Where(x => 0).ToArray();
                //System.Diagnostics.Debug.WriteLine(callbackTimeValue.Last());
                var time_diff = callbackTimeValue.Average();
                var calcFrequency = 1e6 / time_diff;        //1e6 because time is in usec
                
                System.Diagnostics.Debug.WriteLine("callback diff: " + time_diff.ToString());

                if ((calcFrequency >= lowerAllowedFrequency) && (calcFrequency <= upperAllowedFrequency))
                    testResult = true;
                else
                    testResult = false;

                using (StreamWriter writer = new StreamWriter(fileOut, false))
                {
                    if (testResult == true)
                        writer.Write("result = PASS\r\n");
                    else
                        writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = " + time_diff.ToString() + "\r\n");
                    writer.Write("resultParameter3 =\r\n");
                    writer.Write("resultParameter4 =\r\n");
                    writer.Write("resultParameter5 =\r\n");
                    writer.Close();
                }

            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("test results read FAIL: " + ex.Message);
                
                using (StreamWriter writer = new StreamWriter(fileOut, false))
                {
                    writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = exception thrown\r\n");
                    writer.Write("resultParameter2 = null\r\n");
                    writer.Write("resultParameter3 = null\r\n");
                    writer.Write("resultParameter4 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                }
            }

        }
    }
}
