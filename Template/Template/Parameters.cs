using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters	
	public int testTimeout = 60000;
	public string useLogic = "none";
	public double sampleTimeMs = 4000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = false;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = false;
	public string testScriptName = "template.tst";
	public int testScriptTimeoutMs = 9000;
	public string useAnalysis = "none";
	public string analysisScriptName = "none.exe";
	public bool useResultsFile = false;
	public string resultsFileName = "testTemp\\results.txt";
	public int testDelay = 0;
	// Do not change text format above this point
	
	// test specific parameters
	public double testParameter1 = 2.5;
	public double testParameter2 = 2.7;
	public double testParameter3 = 2.3;
	public double lastTestParameter = 2.5;
    }
}
