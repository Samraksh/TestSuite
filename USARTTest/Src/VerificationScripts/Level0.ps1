# This script takes two files as inputs 
# arg1 - Input file of random data that was used in the test
# arg2 - Data received from the mote after testing 
# Author - Nived.Sivadas 

if($args.Length -lt 3)
{
	Write-Host "`n"
	Write-Host "Syntax : Level0.ps1 <InputFile> <OutputFile> <Threshold> <ResultDir>"
	Write-Host "`<InputFile`> - Specify the inputs used during the test"
	Write-Host "`<OutputFile`> - Specify the outputs obtained from the test"
	Write-Host "`<Threshold`> - Give the threshold that determines success of test"
	Write-Host "		Ex. 0.97 mean 97 percent of the strings match from both the files"
	Write-Host "<ResultDir> - Specify where the result must go"
	Write-Host "`n"
	exit
}

$inputFile=$args[0]
$outputFile=$args[1]
$resultDir=$args[3]
$threshold=$args[4]

$results = $resultDir + "Results.txt"

$inputFileHandle = Get-Content $inputFile
$outputFileHandle = Get-Content $outputFile

Compare-Object $inputFileHandle $outputFileHandle | Out-File $results
Compare-Object $inputFileHandle $outputFileHandle -in | %{if($_.sideindicator -eq "<=") {$errors++}}

$totalInput = $inputFileHandle.Length

#Write-Host "Total Input " + $totalInput

$successrate = 1 - $errors/$totalInput

Write-Host "Success Rate " $successrate


if($successrate > $threshold)
{
	$result = "result = SUCCESS"
	$result | out-file -append $results
}
else
{
	$result = "result = FAIL"
	$result | out-file -append $results
}
$testParameter1 = "testParameter1 = "
$testParameter1 | out-file -append $results

$testParamter2 = "testParameter2 = "
$testParameter2 | out-file -append $results

$accuracy = "accuracy = " + $successrate
$accuracy | out-file -append $results


