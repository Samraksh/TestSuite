% reading values from Parameters.cs class
[publicDecl, typeDecl, parameter, equal_sign, value, semiC] = textread('Parameters.cs', '%s %s %s %c %s%c', 'headerlines', 7); 

% extracting parameters and associated values
sampleTimeMsStr = value(2);
sampleFrequencyStr = value(3);
readFrequencyStr = value(18);
upperAllowedFrequencyStr = value(19);
lowerAllowedFrequencyStr = value(20);
expectedFrequencyStr = value(21);

% stripping last ';' character from value
sampleTimeMsStr = strrep(sampleTimeMsStr, ';', '');
sampleFrequencyStr = strrep(sampleFrequencyStr, ';', '');
readFrequencyStr = strrep(readFrequencyStr, ';', '');
upperAllowedFrequencyStr = strrep(upperAllowedFrequencyStr, ';', '');
lowerAllowedFrequencyStr = strrep(lowerAllowedFrequencyStr, ';', '');
expectedFrequencyStr = strrep(expectedFrequencyStr, ';', '');

% converting strings to doubles
sampleTimeMs = str2double(sampleTimeMsStr)
sampleFrequency = str2double(sampleFrequencyStr)
readFrequency = str2double(readFrequencyStr)
upperAllowedFrequency = str2double(upperAllowedFrequencyStr)
lowerAllowedFrequency = str2double(lowerAllowedFrequencyStr)
expectedFrequency = str2double(expectedFrequencyStr)

% dataFileName given by TestRig variable set function
input = csvread (dataFileName,1,0);

SampleNo = input(:,1);
value = input(:,2);
edges = find(diff(value));
time_diff = mean(diff(SampleNo));
time_period = time_diff / sampleFrequency;
period = 2*mean(diff(edges));
calcFrequency = 1/(period * time_period);

if ( (calcFrequency <= upperAllowedFrequency) && (calcFrequency >= lowerAllowedFrequency))
	result = 'PASS';
else
	result = 'FAIL';
end

accuracy = expectedFrequency / calcFrequency;

% setting return values that will be read by TestRig
resultParameter1 = sprintf('%d', calcFrequency);
resultParameter2 = sprintf('%f', expectedFrequency);
resultParameter3 = sprintf('%f', upperAllowedFrequency);
resultParameter4 = sprintf('%f', lowerAllowedFrequency);
resultParameter5 = '';
