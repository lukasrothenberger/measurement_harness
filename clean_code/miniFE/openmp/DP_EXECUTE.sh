cd src
./miniFE.x --nx 100 --ny 100 --nz 100 > dp_execute_out.txt
grep ".*e-08" dp_execute_out.txt
retval=$?
cd ..
return $retval
