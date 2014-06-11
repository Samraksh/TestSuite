using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;

namespace LogicAnalysis
{
    class Program
    {
        const string fileName = @"testTemp\testData.csv";
        const string fileOut = @"testTemp\results.txt";
        const float sampleFreq = 4000000;

        const float expectedFreq0 = 2.5f;
        const float expectedFreq1 = 0f;
        const float expectedFreq2 = 0f;
        const float expectedFreq3 = 0f;

        static int listNumber = 0;
        static List<int> listTime = new List<int>();
        static List<int> line0 = new List<int>();
        static List<int> line1 = new List<int>();
        static List<int> line2 = new List<int>();
        static List<int> line3 = new List<int>();

        static void ReadInFile(int ignoreFirstLineCnt)
        {
            var reader = new StreamReader(File.OpenRead(fileName));
            for (int i = 0; i < ignoreFirstLineCnt; i++)
            {
                // purge out first lines which tends to throw off calculations
                reader.ReadLine();
            }
            while (!reader.EndOfStream)
            {
                var line = reader.ReadLine();
                var values = line.Split(',');

                listNumber = values.Length;

                listTime.Add(int.Parse(values[0]));
                line0.Add(int.Parse(values[1]));
                if (listNumber > 2)
                    line1.Add(int.Parse(values[2]));
                if (listNumber > 3)
                    line2.Add(int.Parse(values[3]));
                if (listNumber > 4)
                    line3.Add(int.Parse(values[4]));
            }
        }

        static float GetFrequency(List<int> time, List<int> line, int startPoint, int stopPoint)
        {
            float calcFreq = 0;

            // looking for first 0 to 1 transistion
            int samplePoint = startPoint;
            bool pointFound = false;
            while ( (samplePoint < stopPoint) && (pointFound == false))
            {
                if (line[samplePoint] == 1)
                {
                    pointFound = true;
                    break;
                }
                samplePoint++;
            }
            System.Diagnostics.Debug.WriteLine("Found first point at " + samplePoint.ToString());

            int totalTime = 0;
            int totalTransistions = 0;
            int lastPoint = line[samplePoint];
            int lastTransitionTime = time[samplePoint];
            for (int i = samplePoint + 1; i < stopPoint; i++)
            {
                // looking for transition from 0 to 1
                if ((line[i] == 1) && (lastPoint == 0))
                {
                    // found transition
                    //System.Diagnostics.Debug.WriteLine("Found first point at " + i.ToString());
                    totalTime += time[i] - lastTransitionTime;
                    totalTransistions++;
                    lastTransitionTime = time[i];
                }
                lastPoint = line[i];
            }

            if ((totalTransistions != 0) && (totalTime != 0))
                calcFreq = sampleFreq / (totalTime / totalTransistions);

            return calcFreq;
        }

        static int FindTransitionToHigh(List<int> time, List<int> line, int initialTime)
        {
            int returnVal = 0;
            int index = 1;

            // Find first entry after initialTime
            while ( (index < time.Count) && (time[index] < initialTime)){
                index++;
            }

            // Find transition to high
            while ((index < time.Count))
            {
                if ((line[index - 1] == 0) && (line[index] == 1))
                    break;
                index++;
            }

            if (index < time.Count)
            {
                returnVal = time[index];
            }
            else
            {
                returnVal = 0;
            }

            return returnVal;
        }

        static void Main(string[] args)
        {
            bool testResult = false;
            try
            {
                ReadInFile(20);
                System.Diagnostics.Debug.WriteLine("Read in " + listTime.Count.ToString() + " from file " + fileName);               

                if (listNumber != 3)
                {
                    System.Diagnostics.Debug.WriteLine("Error in Main");
                    using (StreamWriter writer = new StreamWriter(fileOut))
                    {
                        writer.Write("result = FAIL\r\n");
                        writer.Write("accuracy = 0\r\n");
                        writer.Write("resultParameter1 = Bad data format in reading from file. Perhaps no transitions occurred.\r\n");
                        writer.Write("resultParameter2 = null\r\n");
                        writer.Write("resultParameter3 = null\r\n");
                        writer.Write("resultParameter4 = null\r\n");
                        writer.Write("resultParameter5 = null\r\n");
                        return;
                    }
                }

                // Get 0 -> 1 transition on line1 from the beginning 
                int transitionTime = FindTransitionToHigh(listTime, line1, 0);
                if (transitionTime == 0)
                {
                    System.Diagnostics.Debug.WriteLine("Error in Main");
                    using (StreamWriter writer = new StreamWriter(fileOut))
                    {
                        writer.Write("result = FAIL\r\n");
                        writer.Write("accuracy = 0\r\n");
                        writer.Write("resultParameter1 = Failure to find transition to high.\r\n");
                        writer.Write("resultParameter2 = null\r\n");
                        writer.Write("resultParameter3 = null\r\n");
                        writer.Write("resultParameter4 = null\r\n");
                        writer.Write("resultParameter5 = null\r\n");
                        return;
                    }
                }

                // Find 0 -> 1 transition on line1 from the transition time found above
                int gapTime = FindTransitionToHigh(listTime, line0, transitionTime);

                float timeToInterrupt = ((float)gapTime - (float)transitionTime) / 4000000;

                if ( (timeToInterrupt < 0.01) && (timeToInterrupt > 0) )
                {
                    testResult = true;
                }
                else
                {
                    testResult = false;
                }

                using (StreamWriter writer = new StreamWriter(fileOut,false))
                {
                    if (testResult == true)
                        writer.Write("result = PASS\r\n");
                    else
                        writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = " + (timeToInterrupt / 0.0045) + "\r\n");
                    writer.Write("resultParameter2 = " + timeToInterrupt.ToString() + "\r\n");
                    writer.Write("resultParameter3 = null\r\n");
                    writer.Write("resultParameter4 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                    writer.Close();
                }
            }
            catch
            {
                System.Diagnostics.Debug.WriteLine("Error in Main");
                using (StreamWriter writer = new StreamWriter(fileOut))
                {
                    writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = Exception thrown in logic analysis program.\r\n");
                    writer.Write("resultParameter2 = null\r\n");
                    writer.Write("resultParameter3 = null\r\n");
                    writer.Write("resultParameter4 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                }
            }
        }
    }
}
