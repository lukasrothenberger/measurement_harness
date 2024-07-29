all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner

clean: 
	rm -rvf CG_*

copy_code:
	cp -r ../../../clean_code/NPB-SER CG_orig

execute_hotspot_detection:
	cd CG_orig && make clean
	cd CG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make cg CLASS=S
	cd CG_orig/bin && ./cg.S
	cd CG_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd CG_orig && make clean
	cd CG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make cg CLASS=S
	cd CG_orig/bin && ./cg.S
	cd CG_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd CG_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd CG_orig && mv DP_COMPILE_CG.sh DP_COMPILE.sh
	cd CG_orig && mv DP_EXECUTE_CG.sh DP_EXECUTE.sh
	cd CG_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop