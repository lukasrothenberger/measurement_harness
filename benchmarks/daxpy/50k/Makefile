CC=clang
CXX=clang++
CPU_CC_FLAGS="-fopenmp"
CPU_CXX_FLAGS="-fopenmp"
GPU_CC_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
GPU_CXX_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
DP_OPTIMIZER_FLAGS=--doall-microbench-file ../../../../configuration/doall_1.json --reduction-microbench-file ../../../../configuration/reduction.json --system-configuration ../../../../configuration/system_configuration.json

cpu: clean setup_env cpu_measure_sequential cpu_get_discopop_patterns cpu_measure_optima 

cpu_get_discopop_patterns:
	make reset_env
	cd code && cmake -DCMAKE_C_COMPILER=discopop_cc -DCMAKE_CXX_COMPILER=discopop_cxx . && make
	cd code && ./prog
	ls code/.discopop
	cp -r code/.discopop/* clean_dp_folder
	ls -a clean_dp_folder
	cd clean_dp_folder && discopop_explorer --enable-patterns=reduction,doall

gpu_get_discopop_patterns:
	make reset_env
	cd code && cmake -DCMAKE_C_COMPILER=discopop_cc -DCMAKE_CXX_COMPILER=discopop_cxx . && make
	cd code && ./prog
	rm -rvf clean_dp_folder
	cp -r code/.discopop clean_dp_folder
	cd clean_dp_folder && discopop_explorer --enable-patterns=reduction,doall,simplegpu

cpu_measure_sequential:
	make reset_env
	mkdir -p logs/sequential
	cd code && cmake -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_FLAGS=${CPU_CC_FLAGS} -DCMAKE_CXX_FLAGS=${CPU_CXX_FLAGS} . && make
	/usr/bin/time --format="sequential;%e;%x;" --append --output=measurements.csv code/prog 1>> logs/sequential/stdout.txt 2>> logs/sequential/stderr.txt

cpu_measure_exhaustive_optimum:
	make reset_env
	cd dp_folder && pwd && discopop_optimizer -v -x ${DP_OPTIMIZER_FLAGS}
	cd dp_folder && discopop_patch_generator -v -fcf optimizer/updated_configuration.json
	cd dp_folder && discopop_patch_applicator -v -fcf optimizer/updated_configuration.json
	mkdir -p logs/exhaustive
	cd code && cmake -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_FLAGS=${CPU_CC_FLAGS} -DCMAKE_CXX_FLAGS=${CPU_CXX_FLAGS} . && make
	/usr/bin/time --format="exhaustive;%e;%x;" --append --output=measurements.csv code/prog 1>> logs/exhaustive/stdout.txt 2>> logs/exhaustive/stderr.txt

cpu_measure_optima: cpu_measure_exhaustive_optimum

setup_env:
	echo "ID; time; return_code;" > measurements.csv
	mkdir clean_dp_folder

reset_env:
	echo "RESET ENV"
	rm -rvf code
	cp -r ../../../clean_code/daxpy/50k code
	rm -rvf dp_folder
	cp -r clean_dp_folder dp_folder

clean: 
	rm -rvf code
	rm -rvf clean_dp_folder
	rm -rvf dp_folder
	rm -rvf measurements.csv
	rm -rvf logs




