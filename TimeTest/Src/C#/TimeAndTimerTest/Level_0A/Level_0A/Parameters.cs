using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters
	public int testTimeout = 780000;
	public string useLogic = "normal";
	public double sampleTimeMs = 10000;
	public double sampleFrequency = 8000000;
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
	// Do not change text format above this point
	
	// test specific parameters
	public double frequency = 100;
	public double upperAllowedFrequency = 100.1;
	public double lowerAllowedFrequency = 99.9;
	public double expectedFrequency = 100;
    }
}
