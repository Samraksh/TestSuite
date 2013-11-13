Write-Host "/******************   Welcome to Test Creatorrr  *******************/"

Write-Host "Enter the test source directory"
$SRCDIR = Read-Host

$TESTCREATE = "$SRCDIR/TestSuite/tools/testCreate"

$TEMPLATEFILEDIR = "$TESTCREATE/TemplateFiles"

Write-Host "Enter the name  of the test you wish to create"
$TESTNAME = Read-Host

Write-Host "Enter the level of test you are about to create"
$TESTLEVEL = Read-Host

$TESTDIRNAME = "$SRCDIR/TestSuite/$TESTNAME/Src/Native/$TESTLEVEL"

$TESTDESIGNDOCS = "$SRCDIR/TestSuite/$TESTNAME/DesignDocuments"

$TESTSRCPATH = "$SRCDIR/TestSuite/$TESTNAME/Src"

if(!(Test-Path -Path $TESTDESIGNDOCS))
{
	New-Item -ItemType directory -Path $TESTDESIGNDOCS
}

if(!(Test-Path -Path $TESTSRCPATH))
{
	New-Item -ItemType directory -Path $TESTSRCPATH
}

if(!(Test-Path -Path $TESTDIRNAME))
{
	New-Item -ItemType directory -Path $TESTDIRNAME
}
else
{
	if((Test-Path -Path "$TESTDIRNAME\$TESTNAME.cpp"))
	{
		Write-Host "A test already exists with this name"
		exit
	}
}

# Copy the entry point to the final test
Copy-Item $TEMPLATEFILEDIR\EntryPoint.cpp $TESTDIRNAME

# Copy the scatter file to the final folder
Copy-Item $TEMPLATEFILEDIR\scatterfile_tools_gcc.xml $TESTDIRNAME

Copy-Item $TEMPLATEFILEDIR\signalr.c $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\syscalls.c $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\fixedPoint.h $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\testMath.h $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\Parameters.h $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\test.c $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\template.edf $TESTDIRNAME\$TESTNAME.edf
Copy-Item $TEMPLATEFILEDIR\template.hkp $TESTDIRNAME\$TESTNAME.hkp
New-Item -ItemType directory -Path $TESTDIRNAME\GNU_S
Copy-Item $TEMPLATEFILEDIR\GNU_S\tinyclr_vector.cpp $TESTDIRNAME\GNU_S

Get-Content ("$TEMPLATEFILEDIR\template.cpp") | ForEach-Object {$_ -replace 'TEMPLATE',$TESTNAME} | Set-Content  ("$TESTDIRNAME\$TESTNAME.cpp")
Get-Content ("$TEMPLATEFILEDIR\template.h") | ForEach-Object {$_ -replace 'TEMPLATE',$TESTNAME} | Set-Content  ("$TESTDIRNAME\$TESTNAME.h")
Get-Content ("$TEMPLATEFILEDIR\template.proj") | ForEach-Object {$_ -replace 'TEMPLATE',$TESTNAME} | Set-Content  ("$TESTDIRNAME\$TESTNAME.proj")
Get-Content ("$TEMPLATEFILEDIR\tests.xml") | ForEach-Object {$_ -replace 'TEMPLATE',$TESTNAME} | Set-Content  ("$TESTDIRNAME\tests.xml")

Write-Host "/*********************  Test Creation is complete *************************/"

