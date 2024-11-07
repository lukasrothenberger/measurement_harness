cd openmp/kmeans
make clean
make CC=clang  CC_FLAGS="-g -fopenmp -O2 -fsanitize=thread" OPENMP/kmeans