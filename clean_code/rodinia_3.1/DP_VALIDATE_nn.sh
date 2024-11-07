cd openmp/nn
./nn filelist_4 5 30 90 > dp_execute_out.txt
grep "1988 12 27  0 18 TONY" dp_execute_out.txt
retval=$?
cd ..
return $retval