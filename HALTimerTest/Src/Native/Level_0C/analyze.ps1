#	Author : nived.sivadas
#       Description : This script is designed to help with the verification of results 
#		      generated as the output of the logic analyzer 
#       Output      : Generates a testOutput file which can be consumed by the logic analyzer
#
#

#Period set on timer1 
$timer1Period = 20000 

#Period set on timer2
$timer2Period = 110000

# The acceptable error in percentage
$acceptableError = 10

# This variable represents the sampling frequency of the logic
$samplesPerUSecond = 4

# determines if the test was successful
$acceptedAccuracy = 10

$totalSamplestimer1 = 0
$totalSamplestimer2 = 0

$errSamplestimer1 =  0
$errSamplestimer2 =  0

$workingDirectory=$pwd.Path

Write-Host $workingDirectory

$resultFile = $workingDirectory + "\results.txt"

$dataFile = $workingDirectory + "\testTemp\testData.csv"

if(!(Test-Path $dataFile))
{
	exit
}
	

$csvDll = $workingDirectory + "\CsvReader.dll"

Write-Host $csvDll

[System.Reflection.assembly]::LoadFile($workingDirectory + "\CsvReader.dll")

$csvReaderObject = New-Object FileReader.CsvReader($dataFile)

$dataTable = $csvReaderObject.GetTable()

$lastSampleNumbertimer1 = 0
$lastSampleNumbertimer2 = 0
$lastSampleNumbertimer3 = 0

$interested = 0 
$sleeping = 0

function Abs($value)
{
	if($value -lt 0) { -$Value} else { $value}
}

foreach($row in $dataTable.Rows)
{
	
	if($row[2] -eq 1)
	{
		$timeDifference = ($row[0] - $lastSampleNumbertimer1)	
		$timeDifferenceInSec = $timeDifference / $samplesPerUSecond

		if(($timeDifferenceInSec - $timer1Period) -gt (($acceptableError / 100) * $timer1Period))
		{
			$sleeping = 1;
		}
		
		if(($timeDifferenceInSec - $timer1Period) -gt (($acceptableError / 100) * $timer1Period) -and ($row[1] -eq  0))
		{
			$errSamplestimer1++
		}
		$totalSamplestimer1++  
		
		$lastSampleNumbertimer1 = $row[0]
	}

}

Write-Host "Total Measurements  "  $totalSamplestimer1
Write-Host "Error Samples "  $errSamplestimer1
$accuracytimer1 =  ($errSamplestimer1 / $totalSamplestimer1)

if((($accuracytimer1 * 100) -gt $acceptedAccuracy) -or ($sleeping -eq 0))
{
	$result = "FAIL"
}
else
{
	$result = "PASS"
}


"result=" + $result  | Out-File -FilePath $resultFile -Append
"accuracy=" + (1 - $accuracytimer1)  | Out-File -FilePath $resultFile -Append
if($result -eq "PASS")
{
"resultParameter1=Test was successful" | Out-File -FilePath $resultFile -Append
}
else
{
"resultParameter1=Test failed because of low accuracy" | Out-File -FilePath $resultFile -Append
}
"resultParameter2=" | Out-File -FilePath $resultFile -Append
"resultParameter3=" | Out-File -FilePath $resultFile -Append
"resultParameter4=" | Out-File -FilePath $resultFile -Append
"resultParameter5=" | Out-File -FilePath $resultFile -Append

