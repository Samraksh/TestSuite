using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters
	public int testTimeout = 10000;
	public string useLogic = "normal";
	public double sampleTimeMs = 5000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = false;
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

    }
}
