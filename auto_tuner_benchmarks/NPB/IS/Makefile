all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner

clean: 
	rm -rvf IS_*

copy_code:
	cp -r ../../../clean_code/NPB-SER IS_orig

execute_hotspot_detection:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make is CLASS=S
	cd IS_orig/bin && ./is.S
	cd IS_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make is CLASS=S
	cd IS_orig/bin && ./is.S
	cd IS_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd IS_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd IS_orig && mv DP_COMPILE_IS.sh DP_COMPILE.sh
	cd IS_orig && mv DP_EXECUTE_IS.sh DP_EXECUTE.sh
	cd IS_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop --skip-cleanup