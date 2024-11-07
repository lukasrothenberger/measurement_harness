mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

all: cpu

cpu: clean copy_code execute_dynamic_analysis 
clean: 
	rm -rvf ${current_dir}_*

copy_code:
	cp -r ../../../clean_code/rodinia_3.1 ${current_dir}_orig

execute_dynamic_analysis:
	cd ${current_dir}_orig && make clean
	cd ${current_dir}_orig && DP_PROJECT_ROOT_DIR=$$(pwd) DOT_DISCOPOP=$$(pwd)/.discopop CXX=discopop_cxx CC=discopop_cc ./DP_COMPILE_${current_dir}.sh
	cd ${current_dir}_orig && DOT_DISCOPOP=$$(pwd)/.discopop ./DP_EXECUTE_${current_dir}_DP.sh
