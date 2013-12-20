#	Author : nived.sivadas
#       Description : This script is designed to help with the verification of results 
#		      generated as the output of the logic analyzer 
#       Output      : Generates a testOutput file which can be consumed by the test rig to ascertain the result
#
#

# This variable describes the sleep time of the function which is 300us
$sleepTime = 1000000

# The acceptable error in percentage
$acceptableError = 10

# This variable represents the sampling frequency of the logic
$samplesPerUSecond = 4

# determines if the test was successful
$acceptedAccuracy = 10

$totalSamples1 = 0

$totalSamples2 = 0

$errSamples - 0

$workingDirectory=$pwd.Path

Write-Host $workingDirectory

$resultFile = $workingDirectory + "\testTemp\results.txt"

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

$lastSampleNumber = 0

function Abs($value)
{
	if($value -lt 0) { -$Value} else { $value}
}

foreach($row in $dataTable.Rows)
{
#	Write-Host $row[0]
#	Write-Host $dataTable.Columns[1]
	if($row[1] -eq 1)
	{
		$totalSamples1++
	}	

	if($row[2] -eq 1)
	{
		$totalSamples2++
	}		
}

if($totalSamples1 -gt 0 -and $totalSamples2 -gt 0)
{
	$result = "PASS"
}
else
{
	$result = "FAIL"
}


"result=" + $result  | Out-File -FilePath $resultFile -Append
"accuracy=" + (1 - $accuracy) | Out-File -FilePath $resultFile -Append
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

