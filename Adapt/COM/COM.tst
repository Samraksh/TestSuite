# test script comments start with '#'
# execute .exe|.dll|.sh timeout
execute calc.exe 1000
# sleep ms
sleep 5000
# COM_send random seed number lower_bound upper_bound
COM_send random none 20 0 100
sleep 1000
# COM_send file file_name
COM_send file data.txt
sleep 1000
COM_receive file true rx_data.txt
sleep 1000
COM_send string This is my string.
sleep 2000
COM_receive file false null
sleep 6000
#execute test.ps1 5000
