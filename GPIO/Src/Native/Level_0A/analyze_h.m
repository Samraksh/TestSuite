% reading values from Parameters.cs class
[defineStr, parameter, value] = textread('Parameters.h', '%s %s %s', 'headerlines', 1); 

% extracting parameters and associated values
sampleTimeMsStr = value(3);
sampleFrequencyStr = value(4);
readFrequencyStr = value(20);
upperAllowedFrequencyStr = value(21);
lowerAllowedFrequencyStr = value(22);
expectedFrequencyStr = value(23);

% stripping last ';' character from value
sampleTimeMsStr = strrep(sampleTimeMsStr, ';', '');
sampleFrequencyStr = strrep(sampleFrequencyStr, ';', '');
readFrequencyStr = strrep(readFrequencyStr, ';', '');
upperAllowedFrequencyStr = strrep(upperAllowedFrequencyStr, ';', '');
lowerAllowedFrequencyStr = strrep(lowerAllowedFrequencyStr, ';', '');
expectedFrequencyStr = strrep(expectedFrequencyStr, ';', '');

% converting strings to doubles
sampleTimeMs = str2double(sampleTimeMsStr);
sampleFrequency = str2double(sampleFrequencyStr);
readFrequency = str2double(readFrequencyStr);
upperAllowedFrequency = str2double(upperAllowedFrequencyStr);
lowerAllowedFrequency = str2double(lowerAllowedFrequencyStr);
expectedFrequency = str2double(expectedFrequencyStr);

% dataFileName given by TestRig variable set function
%dataFileName = 'testData.csv'
input = csvread ('testData.csv',1,0);

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
resultParameter3 = '';
resultParameter4 = '';
resultParameter5 = '';

