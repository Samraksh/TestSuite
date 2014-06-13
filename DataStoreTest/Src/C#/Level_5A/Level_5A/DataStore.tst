# waiting for all the exceptions to occur before listening over the COM port
# since an exception message will automatically fail the test
sleep 10000
# starting to receive the COM messages looking for PASS / FAIL message now
COM_receive file enable testTemp\test_results.txt
sleep 3000
COM_receive file disable testTemp\test_results.txt

