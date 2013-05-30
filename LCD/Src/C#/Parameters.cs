using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters	
	// parameters used to gather data
	public int testTimeout = 60000;
	public bool useLogic = false;
	public double sampleTimeMs = 15000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = false;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	// parameters used to run test
	public bool useTestScript = false;
	public string testScriptName = "";
	public int testScriptTimeoutMs = 9000;
	// parameters used to analyze data
	public bool useMatlabAnalysis = false;
	public string matlabScriptName = "analyze.m";
	public bool usePowershellAnalysis = false;
	public string powershellName = "analyze.ps1";	
	// Do not change text format above this point
    }
}
