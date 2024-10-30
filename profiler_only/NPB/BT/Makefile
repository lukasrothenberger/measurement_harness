all: cpu

cpu: clean copy_code execute_dynamic_analysis

clean: 
	rm -rvf BT_*

copy_code:
	cp -r ../../../clean_code/NPB-SER BT_orig

execute_dynamic_analysis:
	cd BT_orig && make clean
	cd BT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make bt CLASS=S
	cd BT_orig/bin && ./bt.S
