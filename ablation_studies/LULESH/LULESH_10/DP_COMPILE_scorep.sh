rm -rf dp_autotuner_build
mkdir dp_autotuner_build
cd dp_autotuner_build
cmake -DWITH_MPI=Off -DWITH_OPENMP=OFF -DCMAKE_C_FLAGS="-fopenmp -O3" -DCMAKE_CXX_FLAGS="-fopenmp -O3" .. -DCMAKE_C_COMPILER=scorep-gcc -DCMAKE_CXX_COMPILER=scorep-g++
make
