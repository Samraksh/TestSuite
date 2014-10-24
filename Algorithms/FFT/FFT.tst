# test script comments start with '#'
# execute <executable> <timeout ms>
# COM_receive <enable/disable> <file name>
COM_receive file enable rx_data.txt
sleep 40000
# COM_receive <enable/disable> <file name>
COM_receive file disable rx_data.txt
# file <enable/disable> <file name>
test_result file compare local_data.txt rx_data.txt
