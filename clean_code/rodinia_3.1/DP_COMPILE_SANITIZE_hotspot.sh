cd openmp/hotspot

make clean
CC=clang  CC_FLAGS="-g -fopenmp -O2 -fsanitize=thread" make