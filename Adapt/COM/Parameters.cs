using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters
	public bool useLogic = false;
	public double sampleTimeMs = 15000;
	public double sampleFrequency = 4000000;
	public bool useExecutable = false;
	public string executableName = "example.exe";
	public int testRunTimeMs = 9000;
	public bool useMatlabAnalysis = false;
	public string matlabScriptName = "analyze.m";
	public bool usePowershellAnalysis = false;
	public string powershellName = "analyze.ps1";
	public bool useCOMPort = true;
	public string forceCOM = "";
	public string COMParameters = "57600,N,8,1";
	// Do not change text format above this point
    }
}
