// required TestRig parameters
#define testTimeout 60000
#define useLogic "normal"
#define sampleTimeMs 10000
#define sampleFrequency 4000000
#define useCOMPort false
#define forceCOM ""
#define COMParameters "115200,N,8,1"
#define useTestScript false
#define testScriptName "none.tst"
#define testScriptTimeoutMs 9000
#define useAnalysis "Matlab"
#define	analysisScriptName "analyze_h.m"
#define	useResultsFile false
#define resultsFileName "results.txt"
// Do not change text format above this point

// test specific parameters
#define frequency 50
#define upperAllowedFrequency 64
#define lowerAllowedFrequency 46 
#define expectedFrequency 50
