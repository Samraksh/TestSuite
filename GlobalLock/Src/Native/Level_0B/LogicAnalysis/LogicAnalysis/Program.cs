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

        const float expectedFreq0 = 60f;
        const float expectedFreq1 = 30f;
        const float expectedFreq2 = 16.6f;
        const float expectedFreq3 = 0f;

        static int listNumber = 0;
        static List<int> listTime = new List<int>();
        static List<int> line0 = new List<int>();
        static List<int> line1 = new List<int>();
        static List<int> line2 = new List<int>();
        static List<int> line3 = new List<int>();

        static void ReadInFile()
        {
            var reader = new StreamReader(File.OpenRead(fileName));
            // purge out first line which tends to throw off calculations
            reader.ReadLine();
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

        static int FindGap(List<string> time, List<string> line)
        {
            return 0;
        }

        static void Main(string[] args)
        {
            bool testResult = false;
            bool result0 = true, result1 = true, result2 = true, result3 = true;
            float freq0 = 0, freq1 = 0, freq2 = 0, freq3 = 0;
            try
            {
                ReadInFile();
                System.Diagnostics.Debug.WriteLine("Read in " + listTime.Count.ToString() + " from file " + fileName);
                if (listNumber <= 1)
                {
                    result0 = false;
                    result1 = false;
                    result2 = false;
                    result3 = false;
                }
                if (listNumber > 1)
                {
                    freq0 = GetFrequency(listTime, line0, 0, line0.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 0 " + freq0.ToString());
                    if ((freq0 < expectedFreq0 * 1.1) && (freq0 > expectedFreq0 * 0.9))
                    {
                        result0 = true;
                    } else
                        result0 = false;
                }
                if (listNumber > 2)
                {
                    freq1 = GetFrequency(listTime, line1, 0, line1.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 1 " + freq1.ToString());
                    if ((freq1 < expectedFreq1 * 1.1) && (freq1 > expectedFreq1 * 0.9))
                    {
                        result1 = true;
                    } else
                        result1 = false;
                }
                if (listNumber > 3)
                {
                    freq2 = GetFrequency(listTime, line2, 0, line2.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 2 " + freq2.ToString());
                    if ((freq2 < expectedFreq2 * 1.1) && (freq2 > expectedFreq2 * 0.9))
                    {
                        result2 = true;
                    } else
                        result2 = false;
                }
                if (listNumber > 4)
                {
                    freq3 = GetFrequency(listTime, line3, 0, line3.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 3 " + freq3.ToString());
                    if ((freq3 < expectedFreq3 * 1.1) && (freq3 > expectedFreq3 * 0.9))
                    {
                        result3 = true;
                    } else
                        result3 = false;
                }

                if ((result0 == false) || (result1 == false) || (result2 == false) || (result3 == false))
                {
                    testResult = false;
                }
                else
                {
                    testResult = true;
                }

                using (StreamWriter writer = new StreamWriter(fileOut,false))
                {
                    if (testResult == true)
                        writer.Write("result = PASS\r\n");
                    else
                        writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 1\r\n");
                    writer.Write("resultParameter1 = " + freq0.ToString() + "\r\n");
                    writer.Write("resultParameter2 = " + freq1.ToString() + "\r\n");
                    writer.Write("resultParameter3 = " + freq2.ToString() + "\r\n");
                    writer.Write("resultParameter4 = " + freq3.ToString() + "\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                    writer.Close();
                }
            }
            catch
            {
                System.Diagnostics.Debug.WriteLine("Error in Main");
                using (StreamWriter writer = new StreamWriter(fileOut, false))
                {
                    writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = " + freq0.ToString() + "\r\n");
                    writer.Write("resultParameter2 = " + freq1.ToString() + "\r\n");
                    writer.Write("resultParameter3 = null\r\n");
                    writer.Write("resultParameter4 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                }
            }
        }
    }
}
