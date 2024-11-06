BENCHMARK_NAME=$$(basename ${CURDIR})
BENCHMARK_NAME_LOWERCASE=$$(echo ${BENCHMARK_NAME} | tr '[:upper:]' '[:lower:]')

all: cpu

cpu: clean copy_code execute_dynamic_analysis

clean: 
	rm -rvf ${BENCHMARK_NAME}_*

copy_code:
	cp -r ../../../clean_code/NPB-SER ${BENCHMARK_NAME}_orig

execute_dynamic_analysis:
	cd ${BENCHMARK_NAME}_orig && make clean
	cd ${BENCHMARK_NAME}_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make ${BENCHMARK_NAME_LOWERCASE} CLASS=S
	cd ${BENCHMARK_NAME}_orig/bin && ./${BENCHMARK_NAME_LOWERCASE}.S
