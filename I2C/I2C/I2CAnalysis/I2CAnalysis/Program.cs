using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;

namespace I2CAnalysis
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
            matches[2] = "EE EF 8D ED";
            matches[3] = "8F 8E CD 6E";
			matches[4] = "EE EF 8D ED";
            matches[5] = "8F 8E CD 6E";
            matches[6] = "EE EF 8D ED";
            matches[7] = "8F 8E CD 6E";
			matches[9] = "EE EF 8D ED";
            matches[10] = "8F 8E CD 6E";

            try
            {
                using (StreamReader reader = new StreamReader(fileName))
                {
                    string line;
                    bool foundChars;

                    line = reader.ReadLine();
                    while ((line != null) && (parameterNum < 10))
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
