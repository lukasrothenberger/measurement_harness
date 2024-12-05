cd src
make clean
make -f Makefile.gnu.openmp
cd ..

cd src
./miniFE.x --nx 15 --ny 15 --nz 15 --verify_solution=1 > dp_execute_out.txt
grep "solution matches analytic solution" dp_execute_out.txt
retval=$?
cd ..
return $retval
