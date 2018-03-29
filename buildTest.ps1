param (
    [Parameter(HelpMessage="Specify solution build: stm32|adapt|smartfusion2")][string]$solution = 'SmartFusion2',
	[Parameter(HelpMessage="Specify TestDir to build: TinyCLR|TinyBooter")][String]$testDir = "VirtualTimerTest\Src\Native_RTC\Level_0A",
    [Parameter(HelpMessage="Specify TestName to build: TinyCLR|TinyBooter")][String]$testName = "Test",
    [Parameter(HelpMessage="Specify action: build|clean: ")][string]$action = "build"
)

write-host "Welcome to Samraksh Emote TestSuite"

$curPath=$pwd.ToString()

$eMoteRepo=$curPath+ "\..\eMote\"
write-host "eMote Repo path is $eMoteRepo"
$gcc_ver="6.3.1"


$envScript = $eMoteRepo + "MicroFrameworkPK_v4_3\Solutions\setEnv.ps1 " + $gcc_ver

iex $envScript 

if (-Not $?) { "command failed"; exit }

$projPath= $curPath+"\"+$testDir
$buildCmd = "msbuild /t:" + $action + " " + $testName + ".proj /p:TSOLUTION=" + $solution
Write-host "Build CMD:::: $buildCmd"
Write-host "Test path: $projPath"

cd $projPath
iex $buildCmd
cd $curPath