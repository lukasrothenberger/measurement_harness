cd dp_autotuner_build
./lulesh2.0 -s 10 > dp_execute_out.txt
grep "MaxRelDiff.*e-14" dp_execute_out.txt
return $?
