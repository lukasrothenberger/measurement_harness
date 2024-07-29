all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner

clean: 
	rm -rvf SP_*

copy_code:
	cp -r ../../../clean_code/NPB-SER SP_orig

execute_hotspot_detection:
	cd SP_orig && make clean
	cd SP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make sp CLASS=S
	cd SP_orig/bin && ./sp.S
	cd SP_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd SP_orig && make clean
	cd SP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make sp CLASS=S
	cd SP_orig/bin && ./sp.S
	cd SP_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd SP_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd SP_orig && mv DP_COMPILE_SP.sh DP_COMPILE.sh
	cd SP_orig && mv DP_EXECUTE_SP.sh DP_EXECUTE.sh
	cd SP_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop