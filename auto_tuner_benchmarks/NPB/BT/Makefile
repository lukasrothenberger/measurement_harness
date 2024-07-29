all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner

clean: 
	rm -rvf BT_*

copy_code:
	cp -r ../../../clean_code/NPB-SER BT_orig

execute_hotspot_detection:
	cd BT_orig && make clean
	cd BT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make bt CLASS=S
	cd BT_orig/bin && ./bt.S
	cd BT_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd BT_orig && make clean
	cd BT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make bt CLASS=S
	cd BT_orig/bin && ./bt.S
	cd BT_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd BT_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd BT_orig && mv DP_COMPILE_BT.sh DP_COMPILE.sh
	cd BT_orig && mv DP_EXECUTE_BT.sh DP_EXECUTE.sh
	cd BT_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop