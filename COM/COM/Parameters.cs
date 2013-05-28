﻿using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters	
	// parameters used to gather data
	public bool useLogic = false;
	public double sampleTimeMs = 15000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = true;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = true;
	public string testScriptName = "COM.tst";
	public int testScriptTimeoutMs = 9000;
	public bool useMatlabAnalysis = false;
	public string matlabScriptName = "analyze.m";
	public bool usePowershellAnalysis = false;
	public string powershellName = "analyze.ps1";	
	// Do not change text format above this point
    }
}