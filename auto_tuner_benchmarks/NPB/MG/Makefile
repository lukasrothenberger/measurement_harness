all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner

clean: 
	rm -rvf MG_*

copy_code:
	cp -r ../../../clean_code/NPB-SER MG_orig

execute_hotspot_detection:
	cd MG_orig && make clean
	cd MG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make mg CLASS=A
	cd MG_orig/bin && ./mg.A
	cd MG_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd MG_orig && make clean
	cd MG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make mg CLASS=S
	cd MG_orig/bin && ./mg.S
	cd MG_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd MG_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd MG_orig && mv DP_COMPILE_MG.sh DP_COMPILE.sh
	cd MG_orig && mv DP_EXECUTE_MG.sh DP_EXECUTE.sh
	cd MG_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop --skip-cleanup