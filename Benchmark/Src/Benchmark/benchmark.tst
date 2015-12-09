# test script comments start with '#'
# execute <executable> <timeout ms>
#execute calc.exe 1000
# sleep ms. Allow time for test to run.
sleep 1000
# COM_receive <enable/disable> <file name>
COM_receive file enable rx_data.txt
sleep 70000
COM_receive file disable rx_data.txt
sleep 1000