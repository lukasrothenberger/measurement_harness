cd dp_autotuner_build
./lulesh2.0 -s 35 > dp_execute_out.txt
grep "MaxRelDiff.*e-13" dp_execute_out.txt
return $?
