COMMAND=code/prog
CC=clang
CXX=clang++
CPU_CC_FLAGS="-fopenmp"
CPU_CXX_FLAGS="-fopenmp"
GPU_CC_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
GPU_CXX_FLAGS="-fopenmp -fopenmp-targets=nvptx64"

cpu: clean setup_env cpu_measure_sequential

cpu_measure_sequential: reset_env
	mkdir -p logs/sequential
	cd code && cmake -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_FLAGS=${CPU_CC_FLAGS} -DCMAKE_CXX_FLAGS=${CPU_CXX_FLAGS} . && make
	/usr/bin/time --format="sequential;%e;%x;" --append --output=measurements.csv ${COMMAND} 1>> logs/sequential/stdout.txt 2>> logs/sequential/stderr.txt

setup_env:
	echo "ID; time; return_code;" > measurements.csv

reset_env:
	cp -r ../../../clean_code/daxpy/50k code


clean: 
	rm -rvf code
	rm -rvf dp_folder
	rm -rvf logs
	rm -rvf measurements.csv




