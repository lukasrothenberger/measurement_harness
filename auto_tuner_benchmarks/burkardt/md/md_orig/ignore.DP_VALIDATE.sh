rm -rf dp_autotuner_build
mkdir dp_autotuner_build
cd dp_autotuner_build
cmake -DCMAKE_CXX_FLAGS="-fopenmp" ..
make

timeout 20 ./prog 3 100 100 > dp_execute_out.txt
grep "4937.09" dp_execute_out.txt
return $?
