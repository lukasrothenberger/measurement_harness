all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner execute_parallel_reference

clean: 
	rm -rvf LU_*

copy_code:
	cp -r ../../../clean_code/NPB-SER LU_orig

execute_hotspot_detection:
	cd LU_orig && make clean
	cd LU_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make lu CLASS=S
	cd LU_orig/bin && ./lu.S
	cd LU_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd LU_orig && make clean
	cd LU_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make lu CLASS=S
	cd LU_orig/bin && ./lu.S
	cd LU_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd LU_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd LU_orig && mv DP_COMPILE_LU.sh DP_COMPILE.sh
	cd LU_orig && mv DP_EXECUTE_LU.sh DP_EXECUTE.sh
	cd LU_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP LU_OMP
	cd LU_OMP && ./DP_COMPILE_LU.sh
	cd LU_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../LU_orig/measurements.csv ./DP_EXECUTE_LU.sh

