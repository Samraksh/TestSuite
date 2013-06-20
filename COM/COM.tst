# test script comments start with '#'
# execute <executable> <timeout ms>
# sleep ms
sleep 5000
# file <enable/disable> <file name>
file enable local_data.txt
# COM_receive <enable/disable> <file name>
COM_receive file enable rx_data.txt
sleep 1000
# COM_send random <seed> <number> <lower_bound> <upper_bound>
COM_send random 1 20 0 100
file random 1 20 0 100
sleep 1000
# COM_send file <file_name>
COM_send file data.txt
file file data.txt
sleep 1000
# COM_send string <string>
COM_send string This is my string.
file string This is my string.
sleep 2000
# COM_receive <enable/disable> <file name>
COM_receive file disable rx_data.txt
# file <enable/disable> <file name>
file disable local_data.txt
sleep 2000
test_result file compare local_data.txt rx_data.txt
