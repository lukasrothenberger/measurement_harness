all: clean 3mm_base_1024 3mm_base_512 3mm_base_256 3mm_base_128 atax_base gemm_base gemver_base

3mm_base_1024:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/3mm utilities/polybench.c linear-algebra/kernels/3mm/3mm.c  -o 3mm_base_1024 -DNI=1024 -DNJ=1024 -DNK=1024 -DNL=1024 -DNM=1024

3mm_base_512:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/3mm utilities/polybench.c linear-algebra/kernels/3mm/3mm.c  -o 3mm_base_512 -DNI=512 -DNJ=512 -DNK=512 -DNL=512 -DNM=512
	
3mm_base_256:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/3mm utilities/polybench.c linear-algebra/kernels/3mm/3mm.c  -o 3mm_base_256 -DNI=256 -DNJ=256 -DNK=256 -DNL=256 -DNM=256

3mm_base_128:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/3mm utilities/polybench.c linear-algebra/kernels/3mm/3mm.c  -o 3mm_base_128 -DNI=128 -DNJ=128 -DNK=128 -DNL=128 -DNM=128 -DPOLYBENCH_DUMP_ARRAYS

atax_base:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c  -o atax_base

gemm_base:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/gemm/gemm.c -o gemm_base

gemver_base:
	$(CC) $(CFLAGS) -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/gemver/gemver.c -o gemver_base

clean:
	rm -f 3mm_base_1024 3mm_base_512 3mm_base_256 3mm_base_128
	rm -f atax_base
	rm -f gemm_base
	rm -f gemver_base
