cd openmp/nn

make clean
CFLAGS="-lm -fopenmp -Wall -fsanitize=thread" make 