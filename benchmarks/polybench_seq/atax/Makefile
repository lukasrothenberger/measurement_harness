CC=clang
CXX=clang++
CPU_CC_FLAGS="-fopenmp"
CPU_CXX_FLAGS="-fopenmp"
GPU_CC_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
GPU_CXX_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
DP_OPTIMIZER_FLAGS=--doall-microbench-file ../../../../configuration/doall_1.json --reduction-microbench-file ../../../../configuration/reduction.json

########## CPU + GPU
#all: clean setup_env cpu_prepare_code gpu_prepare_code cpu_compile gpu_compile cpu_measure gpu_measure
all: cpu

########## CPU
cpu: clean setup_env cpu_prepare_code cpu_compile cpu_measure

cpu_prepare_code: 
	make prepare_sequential
	make prepare_cpu_exhaustive_optimum
	make prepare_cpu_evolutionary_optimum

cpu_compile:
	make compile_sequential
	make compile_cpu_exhaustive_optimum
	make compile_cpu_evolutionary_optimum

cpu_measure:
	make measure_sequential
	make measure_cpu_exhaustive_optimum
	make measure_cpu_evolutionary_optimum

cpu_cleanup:
	make cleanup_sequential
	make cleanup_cpu_exhaustive_optimum
	make cleanup_cpu_evolutionary_optimum

##### DISCOPOP 
get_discopop_patterns:
	make reset_env
	cd code && make -f Makefile.discopop instrument_atax
	cd code && ./atax_base
	ls code/.discopop
	cp -r code/.discopop/* clean_dp_folder
	ls -a clean_dp_folder
	cd clean_dp_folder && discopop_explorer --enable-patterns=reduction,doall

##### SEQUENTIAL
cleanup_sequential:
	rm -rvf code_sequential
prepare_sequential: cleanup_sequential
	make reset_env
	mv code code_sequential
	mkdir -p logs/sequential
compile_sequential:
	-cd code_sequential && CC=${CC} CFLAGS=${CPU_CC_FLAGS} make -f Makefile.discopop compile_atax
measure_sequential:
	-/usr/bin/time --format="sequential;%e;%x;" --append --output=measurements.csv code_sequential/atax_base 1>> logs/sequential/stdout.txt 2>> logs/sequential/stderr.txt

##### CPU EXHAUSTIVE
cleanup_cpu_exhaustive_optimum:
	rm -rvf code_cpu_exhaustive_optimum
prepare_cpu_exhaustive_optimum: cleanup_cpu_exhaustive_optimum get_discopop_patterns
	make reset_env
	cd dp_folder && pwd && discopop_optimizer -v -x ${DP_OPTIMIZER_FLAGS} --system-configuration ../../../../configuration/cpu_only_system_configuration.json
	cd dp_folder && discopop_patch_generator -v -fcf optimizer/updated_configuration.json
	cd dp_folder && discopop_patch_applicator -v -fcf optimizer/updated_configuration.json
	mv code code_cpu_exhaustive_optimum
	mkdir -p logs/cpu_exhaustive
compile_cpu_exhaustive_optimum:
	-cd code_cpu_exhaustive_optimum && CC=${CC} CFLAGS=${CPU_CC_FLAGS} make -f Makefile.discopop compile_atax
measure_cpu_exhaustive_optimum:
	-/usr/bin/time --format="cpu_exhaustive;%e;%x;" --append --output=measurements.csv code_cpu_exhaustive_optimum/atax_base 1>> logs/cpu_exhaustive/stdout.txt 2>> logs/cpu_exhaustive/stderr.txt

##### CPU EVOLUTIONARY
cleanup_cpu_evolutionary_optimum:
	rm -rvf code_cpu_evolutionary_optimum 
prepare_cpu_evolutionary_optimum: cleanup_cpu_evolutionary_optimum get_discopop_patterns
	make reset_env
	cd dp_folder && pwd && discopop_optimizer -v ${DP_OPTIMIZER_FLAGS} --system-configuration ../../../../configuration/cpu_only_system_configuration.json
	cd dp_folder && discopop_patch_generator -v -fcf optimizer/updated_configuration.json
	cd dp_folder && discopop_patch_applicator -v -fcf optimizer/updated_configuration.json
	mv code code_cpu_evolutionary_optimum
	mkdir -p logs/cpu_evolutionary
compile_cpu_evolutionary_optimum:
	-cd code_cpu_evolutionary_optimum && CC=${CC} CFLAGS=${CPU_CC_FLAGS} make -f Makefile.discopop compile_atax
measure_cpu_evolutionary_optimum:
	-/usr/bin/time --format="cpu_evolutionary;%e;%x;" --append --output=measurements.csv code_cpu_evolutionary_optimum/atax_base 1>> logs/cpu_evolutionary/stdout.txt 2>> logs/cpu_evolutionary/stderr.txt

#####

setup_env:
	echo "ID; time; return_code;" > measurements.csv
	mkdir clean_dp_folder

reset_env:
	echo "RESET ENV"
	rm -rvf code
	cp -r ../../../clean_code/polybench_seq code
	rm -rvf dp_folder
	mkdir -p clean_dp_folder
	cp -r clean_dp_folder dp_folder

clean:
	rm -rvf code*
	rm -rvf clean_dp_folder
	rm -rvf dp_folder
	rm -rvf measurements.csv
	rm -rvf logs
	make cpu_cleanup



# gcc -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -o atax_base