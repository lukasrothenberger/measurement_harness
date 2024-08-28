all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean: 
	rm -rvf MG_*

copy_code:
	cp -r ../../../clean_code/NPB-SER MG_orig

execute_hotspot_detection:
	cd MG_orig && make clean
	cd MG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make mg CLASS=S
	cd MG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make mg CLASS=A
	cd MG_orig/bin && ./mg.S
	cd MG_orig/bin && ./mg.A
	cd MG_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd MG_orig && make clean
	cd MG_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make mg CLASS=S
	cd MG_orig/bin && ./mg.S

create_code_version_with_metadata:
	cp -r MG_orig MG_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/MG_orig\//\/MG_orig_with_metadata\//g' MG_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r MG_orig MG_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/MG_orig\//\/MG_orig_without_metadata\//g' MG_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd MG_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd MG_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd MG_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd MG_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd MG_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd MG_orig_with_metadata && mv DP_COMPILE_MG.sh DP_COMPILE.sh
	cd MG_orig_with_metadata && mv DP_EXECUTE_MG.sh DP_EXECUTE.sh
	cd MG_orig_with_metadata && echo "$$(pwd)"
	cd MG_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd MG_orig_without_metadata && mv DP_COMPILE_MG.sh DP_COMPILE.sh
	cd MG_orig_without_metadata && mv DP_EXECUTE_MG.sh DP_EXECUTE.sh
	cd MG_orig_without_metadata && echo "$$(pwd)"
	cd MG_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP MG_OMP
	cd MG_OMP && ./DP_COMPILE_MG.sh
	- cd MG_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../MG_orig_with_metadata/measurements.csv ./DP_EXECUTE_MG.sh
	- cd MG_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../MG_orig_without_metadata/measurements.csv ./DP_EXECUTE_MG.sh

	