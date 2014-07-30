using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;

namespace LCDAnalysis
{
    class Program
    {
        static void Main(string[] args)
        {
            const string fileName = @"testTemp\testData.txt";
            const string fileOut = @"testTemp\results.txt";
            string[] matches = new string[11];
            string[] parameter = new string[5];
            int parameterNum = 0;
            int lineMatches = 0;
            bool matchFailure = false;
            bool testResult = false;
            char[] trimChars = new char[] { ' ', ']' };

            // generating test matches
            matches[0] = "EE EF 8D ED";
            matches[1] = "8F 8E CD 6E";
            matches[2] = "60 65 6E 0D";
            matches[3] = "C6 EC ED AE";
            matches[4] = "EA 8C CB E0";
            matches[5] = "6D 29 C5 6E";
            matches[6] = "6A A7 00 00";
            matches[7] = "E7 4F 07 67";
            matches[8] = "00 00 00 00";
            matches[9] = "AF 8E EB 4E";
            matches[10] = "40 41 2F 60";
            
            try
            {
                using (StreamReader reader = new StreamReader(fileName))
                {
                    string line;
                    bool foundChars;

                    line = reader.ReadLine();
                    while ((line != null) && (parameterNum<10))
                    {
                        System.Diagnostics.Debug.WriteLine(line);
                        if (parameterNum < 5)
                        {
                            parameter[parameterNum] = line;
                        }
                        parameterNum++;

                        if (line.StartsWith("[70 80 F0 ") == true) { }
                        else
                        {
                            if (line.StartsWith("[70 80 48 ") == false)
                                testResult = false;
                            line = line.Remove(0, 10);
                            System.Diagnostics.Debug.WriteLine(line);
                            line = line.TrimEnd(trimChars);
                            line = line.Trim();
                            System.Diagnostics.Debug.WriteLine(line);
                            foundChars = false;
                            for (int i = 0; i < 10; i++)
                            {
                                if (line.Equals(matches[i]))
                                {
                                    foundChars = true;
                                    System.Diagnostics.Debug.WriteLine(line + " matches at i: " + i.ToString());
                                    lineMatches++;
                                }
                            }
                            if (foundChars == false)
                                matchFailure = true;
                        }
                        line = reader.ReadLine();
                    }
                    reader.Close();
                }

                if ((lineMatches > 2) && (matchFailure == false))
                    testResult = true;
                else
                    testResult = false;

                using (StreamWriter writer = new StreamWriter(fileOut))
                {
                    if (testResult == true)
                        writer.Write("result = PASS\r\n");
                    else
                        writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = " + parameter[0] + "\r\n");
                    writer.Write("resultParameter2 = " + parameter[1] + "\r\n");
                    writer.Write("resultParameter3 = " + parameter[2] + "\r\n");
                    writer.Write("resultParameter4 = " + parameter[3] + "\r\n");
                    writer.Write("resultParameter5 = " + parameter[4] + "\r\n");
                    writer.Close();
                }

            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("test results read FAIL: " + ex.Message);
                using (StreamWriter writer = new StreamWriter(fileOut))
                {
                    writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = null\r\n");
                    writer.Write("resultParameter2 = null\r\n");
                    writer.Write("resultParameter3 = null\r\n");
                    writer.Write("resultParameter4 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");                    
                }
            }

        }
    }
}
