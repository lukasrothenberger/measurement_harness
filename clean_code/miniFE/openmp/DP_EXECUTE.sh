cd src
./miniFE.x --nx 100 --ny 100 --nz 100 --verify_solution=1 > dp_execute_out.txt
grep "solution matches analytic solution" dp_execute_out.txt
retval=$?
cd ..
return $retval
