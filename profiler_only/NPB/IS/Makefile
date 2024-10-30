all: cpu

cpu: clean copy_code execute_dynamic_analysis

clean: 
	rm -rvf IS_*

copy_code:
	cp -r ../../../clean_code/NPB-SER IS_orig

execute_dynamic_analysis:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make is CLASS=S
	cd IS_orig/bin && ./is.S


