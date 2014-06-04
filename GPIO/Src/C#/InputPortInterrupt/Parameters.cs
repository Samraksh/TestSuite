using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters
	public int testTimeout = 10000;
	public string useLogic = "none";
	public double sampleTimeMs = 10000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = true;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = false;
	public string testScriptName = "";
	public int testScriptTimeoutMs = 9000;
	public string useAnalysis = "none";
	public string analysisScriptName = "LogicAnalysis.exe";
	public bool useResultsFile = false;
	public string resultsFileName = "testTemp\\results.txt";
	// Do not change text format above this point
	
	// test specific parameters
	public double frequency = 2.5;
	public double upperAllowedFrequency = 2.7;
	public double lowerAllowedFrequency = 2.3;
	public double expectedFrequency = 2.5;
    }
}
