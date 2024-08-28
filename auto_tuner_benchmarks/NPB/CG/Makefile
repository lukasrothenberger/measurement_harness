all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean: 
	rm -rvf CG_*

copy_code:
	cp -r ../../../clean_code/NPB-SER CG_orig

execute_hotspot_detection:
	cd CG_orig && make clean
	cd CG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make cg CLASS=S
	cd CG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make cg CLASS=A
	cd CG_orig/bin && ./cg.S
	cd CG_orig/bin && ./cg.A
	cd CG_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd CG_orig && make clean
	cd CG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make cg CLASS=S
	cd CG_orig/bin && ./cg.S

create_code_version_with_metadata:
	cp -r CG_orig CG_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/CG_orig\//\/CG_orig_with_metadata\//g' CG_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r CG_orig CG_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/CG_orig\//\/CG_orig_without_metadata\//g' CG_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd CG_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd CG_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd CG_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd CG_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd CG_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd CG_orig_with_metadata && mv DP_COMPILE_CG.sh DP_COMPILE.sh
	cd CG_orig_with_metadata && mv DP_EXECUTE_CG.sh DP_EXECUTE.sh
	cd CG_orig_with_metadata && echo "$$(pwd)"
	cd CG_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd CG_orig_without_metadata && mv DP_COMPILE_CG.sh DP_COMPILE.sh
	cd CG_orig_without_metadata && mv DP_EXECUTE_CG.sh DP_EXECUTE.sh
	cd CG_orig_without_metadata && echo "$$(pwd)"
	cd CG_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP CG_OMP
	cd CG_OMP && ./DP_COMPILE_CG.sh
	- cd CG_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../CG_orig_with_metadata/measurements.csv ./DP_EXECUTE_CG.sh
	- cd CG_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../CG_orig_without_metadata/measurements.csv ./DP_EXECUTE_CG.sh

	