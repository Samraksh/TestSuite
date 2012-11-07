% reading values from Parameters.cs class
[publicDecl, typeDecl, parameter, equal_sign, value, semiC] = textread('Parameters.cs', '%s %s %f'); 

readFrequency = value(1)
upperAllowedFrequency = value(2)
lowerAllowedFrequency = value(3)
expectedFrequency = value(4)
sampleTimeMs = value(5)
sampleFrequency = value(6)

% dataFileName given by TestRig variable set function
%dataFileName = 'testData.csv';
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
resultParameter3 = '';
resultParameter4 = '';
resultParameter5 = '';

disp(result);
disp(resultParameter1);
disp(resultParameter2);
