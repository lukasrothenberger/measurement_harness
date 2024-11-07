cd openmp/lud

make clean
CFLAGS="-fsanitize=thread" CXXFLAGS="-fsanitize=thread" make lud_omp