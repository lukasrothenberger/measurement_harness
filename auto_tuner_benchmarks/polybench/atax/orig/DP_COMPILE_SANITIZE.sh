# name of the benchmark needs to be appended to the file before use!

CC="clang" CFLAGS="-fopenmp -O1 -fsanitize=thread" make -f Makefile.discopop compile_sanitize_