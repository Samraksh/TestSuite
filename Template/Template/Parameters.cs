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
	public bool useCOMPort = false;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = false;
	public string testScriptName = "template.tst";
	public int testScriptTimeoutMs = 9000;
	public bool useMatlabAnalysis = false;
	public string matlabScriptName = "analyze.m";
	public bool usePowershellAnalysis = false;
	public string powershellName = "analyze.ps1";	
	// Do not change text format above this point
	
	// test specific parameters
	public double frequency = 2.5;
	public double upperAllowedFrequency = 2.7;
	public double lowerAllowedFrequency = 2.3;
	public double expectedFrequency = 2.5;
    }
}