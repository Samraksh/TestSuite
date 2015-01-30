﻿// required TestRig parameters
#define testTimeout 15000
#define useLogic "normal"
#define sampleTimeMs 10000
#define sampleFrequency 4000000
#define useCOMPort false
#define forceCOM ""
#define COMParameters "115200,N,8,1"
#define useTestScript false
#define testScriptName "none.tst"
#define testScriptTimeoutMs 9000
#define useAnalysis "exe"
#define	analysisScriptName "GPIOAnalysis.exe"
#define	useResultsFile true
#define resultsFileName "testTemp\\results.txt"
// Do not change text format above this point

// test specific parameters
#define frequency 5000
#define upperAllowedFrequency 20000
#define lowerAllowedFrequency 1000
#define expectedFrequency 5000
