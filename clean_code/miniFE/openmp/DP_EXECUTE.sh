cd src
./miniFE.x --nx 125 --ny 125 --nz 125 --verify_solution=1 > dp_execute_out.txt
grep "solution matches analytic solution" dp_execute_out.txt
retval=$?
cd ..
return $retval
