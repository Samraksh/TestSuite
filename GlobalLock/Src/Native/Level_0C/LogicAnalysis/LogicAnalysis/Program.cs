﻿using System;
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

        const float expectedFreq0 = 8.97f;
        const float expectedFreq1 = 4.487f;
        const float expectedFreq2 = 16.67f;
        const float expectedFreq3 = 0f;

        const float accuracy = 0.1f;

        static List<double> expectedGapSequence0 = new List<double>() { 0.020, 0.070, 0.020, 0.070 };
        const int USE_EVERY_TRANSITION = 1;
        const int USE_RISING_TRANSITION = 2;

        static int listNumber = 0;
        static List<int> listTime = new List<int>();
        static List<int> line0 = new List<int>();
        static List<int> line1 = new List<int>();
        static List<int> line2 = new List<int>();
        static List<int> line3 = new List<int>();

        static List<int> gap0 = new List<int>();
        static List<int> gap1 = new List<int>();
        static List<int> gap2 = new List<int>();
        static List<int> gap3 = new List<int>();

        // These values are to be used to skip a certain number of samples beyond the first before an analysis is made
        const int skipSamples0 = 0;
        const int skipSamples1 = 0;
        const int skipSamples2 = 0;
        const int skipSamples3 = 0;

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
            while ((samplePoint < stopPoint) && (pointFound == false))
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

        static float FindGap(List<int> time, List<int> line, List<double> expectedGapSequence, int startPoint, int stopPoint, int transitionType)
        {
            // looking for first 0 to 1 transistion
            int samplePoint = startPoint;
            bool pointFound = false;
            while ((samplePoint < stopPoint) && (pointFound == false))
            {
                if (line[samplePoint] == 1)
                {
                    pointFound = true;
                    break;
                }
                samplePoint++;
            }
            System.Diagnostics.Debug.WriteLine("Found first point at " + samplePoint.ToString());

            // the fist value in expectedGaps will be referred to as eG1, then eG2, etc....
            // expecting not ALL gaps are the same (then you should use frequency)            
            int lastPoint = line[samplePoint];
            int lastTransitionTime = time[samplePoint];
            float gapTime = 0;
            int currentGapSearchNum = 0;
            int expectedGapNumCnt = expectedGapSequence.Count;
            bool foundNotEg1 = false;
            bool foundTransition = false;
            for (int i = samplePoint + 1; i < stopPoint; i++)
            {
                // looking for transition from 0 to 1
                if ((transitionType == USE_RISING_TRANSITION) && (((line[i] == 1) && (lastPoint == 0))))
                {
                    foundTransition = true;
                }
                else if ((transitionType == USE_EVERY_TRANSITION) && ((((line[i] == 1) && (lastPoint == 0))) || (((line[i] == 0) && (lastPoint == 1)))))
                {
                    foundTransition = true;
                }

                if (foundTransition == true)
                {
                    // found transition
                    gapTime = (time[i] - lastTransitionTime) / sampleFreq;
                    System.Diagnostics.Debug.WriteLine("Gap: " + gapTime.ToString());
                    // now processing gaps to see if they match expectedGaps
                    if (foundNotEg1 == false)
                    {
                        // So we look for the first instance of a gap that does not equal eG1                        
                        if ((gapTime <= (expectedGapSequence[0] * (1+accuracy) )) && (gapTime >= (expectedGapSequence[0] * (1-accuracy))))
                        {
                            foundNotEg1 = true;
                            System.Diagnostics.Debug.WriteLine("Found notEg1 at " + i.ToString());
                        }
                    }
                    else
                    {
                        if ((gapTime <= (expectedGapSequence[currentGapSearchNum] * (1+accuracy))) && (gapTime >= (expectedGapSequence[currentGapSearchNum] * (1-accuracy))))
                        {
                            currentGapSearchNum++;
                        }
                        else
                        {
                            currentGapSearchNum = 0;
                        }
                    }
                    lastTransitionTime = time[i];

                    foundTransition = false;
                }
                lastPoint = line[i];
                if (currentGapSearchNum >= expectedGapNumCnt)
                {
                    return 1;
                }
            }

            return 0;
        }

        static void Main(string[] args)
        {
            bool testResult = false;
            bool result0 = true, result1 = true, result2 = true, result3 = true;
            float return0 = 0, return1 = 0, return2 = 0, return3 = 0;
            string returnStr0 = null, returnStr1 = null, returnStr2 = null, returnStr3 = null;
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
                    return0 = GetFrequency(listTime, line0, skipSamples0, line0.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 0 " + return0.ToString());
                    if ((return0 < expectedFreq0 * (1+accuracy)) && (return0 > expectedFreq0 * (1-accuracy)))
                    {
                        result0 = true;
                    } else
                        result0 = false;
                    returnStr0 = return0.ToString();

                    /*return0 = FindGap(listTime, line0, expectedGapSequence0, 0, line0.Count, USE_EVERY_TRANSITION);
                    if (return0 == 1)
                    {
                        System.Diagnostics.Debug.WriteLine("Found gap sequence for line 0");
                        result0 = true;
                    }
                    else
                    {
                        System.Diagnostics.Debug.WriteLine("Failed to find gap sequence for line 0");
                        result0 = false;
                    }
                    for (int i = 0; i < expectedGapSequence0.Count; i++)
                    {
                        returnStr0 += expectedGapSequence0[i].ToString() + " ";
                    }*/
                }
                if (listNumber > 2)
                {
                    return1 = GetFrequency(listTime, line1, skipSamples1, line1.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 1 " + return1.ToString());
                    if ((return1 < expectedFreq1 * (1+accuracy)) && (return1 > expectedFreq1 * (1-accuracy)))
                    {
                        result1 = true;
                    }
                    else
                        result1 = false;
                    returnStr1 = return1.ToString();
                    /*return1 = FindGap(listTime, line1, expectedGapSequence0, 0, line1.Count);
                    if (return1 == 1)
                    {
                        System.Diagnostics.Debug.WriteLine("Found gap sequence for line 1");
                        result1 = true;
                    }
                    else
                    {
                        System.Diagnostics.Debug.WriteLine("Failed to find gap sequence for line 1");
                        result1 = false;
                    }
                    for (int i = 0; i < expectedGapSequence0.Count; i++)
                    {
                        returnStr1 += expectedGapSequence0[i].ToString() + " ";
                    }*/
                }
                if (listNumber > 3)
                {
                    return2 = GetFrequency(listTime, line2, skipSamples2, line2.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 2 " + return2.ToString());
                    if ((return2 < expectedFreq2 * (1+accuracy)) && (return2 > expectedFreq2 * (1-accuracy) ))
                    {
                        result2 = true;
                    }
                    else
                        result2 = false;
                    returnStr2 = return2.ToString();
                }
                if (listNumber > 4)
                {
                    return3 = GetFrequency(listTime, line3, skipSamples3, line3.Count);
                    System.Diagnostics.Debug.WriteLine("Frequency is line 3 " + return3.ToString());
                    if ((return3 < expectedFreq3 * (1+accuracy)) && (return3 > expectedFreq3 * (1-accuracy)))
                    {
                        result3 = true;
                    }
                    else
                        result3 = false;
                    returnStr3 = return3.ToString();
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
                    writer.Write("resultParameter1 = " + returnStr0 + "\r\n");
                    writer.Write("resultParameter2 = " + returnStr1 + "\r\n");
                    writer.Write("resultParameter3 = " + returnStr2 + "\r\n");
                    writer.Write("resultParameter4 = " + returnStr3 + "\r\n");
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
                    writer.Write("resultParameter1 = " + returnStr0 + "\r\n");
                    writer.Write("resultParameter2 = " + returnStr1 + "\r\n");
                    writer.Write("resultParameter3 = " + returnStr2 + "\r\n");
                    writer.Write("resultParameter4 = " + returnStr3 + "\r\n");
                    writer.Write("resultParameter5 = null\r\n");
                }
            }
        }
    }
}
