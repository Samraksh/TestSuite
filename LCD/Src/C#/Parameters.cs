using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters	
	// parameters used to gather data
	public int testTimeout = 60000;
	public string useLogic = "I2C";
	public double sampleTimeMs = 15000;
	public double sampleFrequency = 800000;
	public bool useCOMPort = false;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = false;
	public string testScriptName = "";
	public int testScriptTimeoutMs = 9000;
	public string useAnalysis = "exe";
	public string analysisScriptName = "LCDAnalysis.exe";
	public bool useResultsFile = true;
	public string resultsFileName = "results.txt";
	// Do not change text format above this point
    }
}
