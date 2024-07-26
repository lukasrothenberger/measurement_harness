all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner

clean: 
	rm -rvf EP_*

copy_code:
	cp -r ../../../clean_code/NPB-SER EP_orig

execute_hotspot_detection:
	cd EP_orig && make clean
	cd EP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make ep CLASS=S
	cd EP_orig/bin && ./ep.S
	cd EP_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd EP_orig && make clean
	cd EP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make ep CLASS=S
	cd EP_orig/bin && ./ep.S
	cd EP_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd EP_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd EP_orig && mv DP_COMPILE_EP.sh DP_COMPILE.sh
	cd EP_orig && mv DP_EXECUTE_EP.sh DP_EXECUTE.sh
	cd EP_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop --skip-cleanup