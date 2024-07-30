all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner execute_parallel_reference

clean: 
	rm -rvf FT_*

copy_code:
	cp -r ../../../clean_code/NPB-SER FT_orig

execute_hotspot_detection:
	cd FT_orig && make clean
	cd FT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make ft CLASS=S
	cd FT_orig/bin && ./ft.S
	cd FT_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd FT_orig && make clean
	cd FT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make ft CLASS=S
	cd FT_orig/bin && ./ft.S
	cd FT_orig/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd FT_orig/bin/.discopop && discopop_patch_generator

execute_auto_tuner:
	cd FT_orig && mv DP_COMPILE_FT.sh DP_COMPILE.sh
	cd FT_orig && mv DP_EXECUTE_FT.sh DP_EXECUTE.sh
	cd FT_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP FT_OMP
	cd FT_OMP && ./DP_COMPILE_FT.sh
	cd FT_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../FT_orig/measurements.csv ./DP_EXECUTE_FT.sh

