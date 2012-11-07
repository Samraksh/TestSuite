% reading values from Parameters.cs class
[publicDecl, typeDecl, parameter, equal_sign, value, semiC] = textread('Parameters.cs', '%s %s %s %c %s%c', 'headerlines', 7); 

% extracting parameters and associated values
sampleTimeMsStr = value(2);
sampleFrequencyStr = value(3);

% stripping last ';' character from value
sampleTimeMsStr = strrep(sampleTimeMsStr, ';', '');
sampleFrequencyStr = strrep(sampleFrequencyStr, ';', '');

% converting strings to doubles
sampleTimeMs = str2double(sampleTimeMsStr)
sampleFrequency = str2double(sampleFrequencyStr)

% dataFileName given by TestRig variable set function
input = csvread (dataFileName,1,0);

% analyze here
testPassed = 'UNKNOWN';

% modify appropriately
if ( testPassed == true )
	result = 'PASS';
else
	result = 'FAIL';
end

% modify appropriately
accuracy = calculatedAccuracy;

% modify appropriately
% setting return values that will be read by TestRig
resultParameter1 = '';
resultParameter2 = '';
resultParameter3 = '';
resultParameter4 = '';
resultParameter5 = '';
