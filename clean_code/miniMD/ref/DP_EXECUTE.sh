./miniMD -nx 40 -ny 40 -nz 40 > dp_execute_out.txt
#./miniFE.x --nx 125 --ny 125 --nz 125 --verify_solution=1 > dp_execute_out.txt
grep "7.017854e-01 -5.671670e+00 7.172867e-01" dp_execute_out.txt
retval=$?
#cd ..
return $retval
