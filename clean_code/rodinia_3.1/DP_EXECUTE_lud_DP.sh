cd openmp/lud
export TSAN_OPTIONS='ignore_noninstrumented_modules=1'
./omp/lud_omp -n 4 -i ../../data/lud/64.dat 