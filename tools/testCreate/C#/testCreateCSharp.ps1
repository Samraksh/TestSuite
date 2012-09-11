Write-Host "/******************   Welcome to Test Creatorrr  *******************/"

Write-Host "Enter the test source directory"
$SRCDIR = Read-Host

$TESTCREATE = "$SRCDIR/TestSys/tools/testCreate"

$TEMPLATEFILEDIR = "$TESTCREATE/TemplateFilesCSharp"

Write-Host "Enter the name  of the test you wish to create"
$TESTNAME = Read-Host

$TESTDIRNAME = "$SRCDIR/TestSys/$TESTNAME/Src/C#"

$TESTDESIGNDOCS = "$SRCDIR/TestSys/$TESTNAME/DesignDocuments"

$TESTSRCPATH = "$SRCDIR/TestSys/$TESTNAME/Src"

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
	if((Test-Path -Path "$TESTDIRNAME\Program.cs"))
	{
		Write-Host "A test already exists with this name"
		exit
	}
}

# Copy the entry point to the final test
Copy-Item $TEMPLATEFILEDIR\Template\Template\Program.cs $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\Template\Template\Resources.Designer.cs $TESTDIRNAME
Copy-Item $TEMPLATEFILEDIR\Template\Template\Resources.resx $TESTDIRNAME

Get-Content ("$TEMPLATEFILEDIR\Template\Template\Template.csproj") | ForEach-Object {$_ -replace 'Template',$TESTNAME} | Set-Content  ("$TESTDIRNAME\$TESTNAME.csproj")

Copy-Item $TEMPLATEFILEDIR\Template\Template\Template.csproj.user  $TESTDIRNAME\$TESTNAME.csproj.user

New-Item -ItemType directory -Path $TESTDIRNAME\Properties

Copy-Item $TEMPLATEFILEDIR\Template\Template\Properties\AssemblyInfo.cs $TESTDIRNAME\Properties


Write-Host "/*********************  Test Creation is complete *************************/"

