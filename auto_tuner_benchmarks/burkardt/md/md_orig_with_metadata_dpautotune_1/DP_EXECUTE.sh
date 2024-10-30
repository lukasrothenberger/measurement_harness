cd dp_autotuner_build
./prog 3 40 40 > dp_execute_out.txt
grep ".*e-11" dp_execute_out.txt
return $?
