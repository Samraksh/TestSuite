﻿using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters
	public int testTimeout = 20000;
	public string useLogic = "normal";
	public double sampleTimeMs = 10000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = true;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = false;
	public string testScriptName = "";
	public int testScriptTimeoutMs = 9000;
	public string useAnalysis = "exe";
	public string analysisScriptName = "LogicAnalysis.exe";
	public bool useResultsFile = true;
	public string resultsFileName = "testTemp\\results.txt";
	public int testDelay = 10000;
	// Do not change text format above this point
	
	// test specific parameters
	public double frequency = 25;
    }
}
