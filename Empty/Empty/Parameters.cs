using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters
	public int testTimeout = 625000;
	public string useLogic = "normal";
	public double sampleTimeMs = 600000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = false;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = false;
	public string testScriptName = "";
	public int testScriptTimeoutMs = 9000;
	public string useAnalysis = "none";
	public string analysisScriptName = "none.exe";
	public bool useResultsFile = false;
	public string resultsFileName = "results.txt";
	public int testDelay = 0;
	// Do not change text format above this point
    }
}
