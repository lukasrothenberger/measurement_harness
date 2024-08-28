all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean: 
	rm -rvf EP_*

copy_code:
	cp -r ../../../clean_code/NPB-SER EP_orig

execute_hotspot_detection:
	cd EP_orig && make clean
	cd EP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make ep CLASS=S
	cd EP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make ep CLASS=A
	cd EP_orig/bin && ./ep.S
	cd EP_orig/bin && ./ep.A
	cd EP_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd EP_orig && make clean
	cd EP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make ep CLASS=S
	cd EP_orig/bin && ./ep.S

create_code_version_with_metadata:
	cp -r EP_orig EP_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/EP_orig\//\/EP_orig_with_metadata\//g' EP_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r EP_orig EP_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/EP_orig\//\/EP_orig_without_metadata\//g' EP_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd EP_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd EP_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd EP_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd EP_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd EP_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd EP_orig_with_metadata && mv DP_COMPILE_EP.sh DP_COMPILE.sh
	cd EP_orig_with_metadata && mv DP_EXECUTE_EP.sh DP_EXECUTE.sh
	cd EP_orig_with_metadata && echo "$$(pwd)"
	cd EP_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd EP_orig_without_metadata && mv DP_COMPILE_EP.sh DP_COMPILE.sh
	cd EP_orig_without_metadata && mv DP_EXECUTE_EP.sh DP_EXECUTE.sh
	cd EP_orig_without_metadata && echo "$$(pwd)"
	cd EP_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP EP_OMP
	cd EP_OMP && ./DP_COMPILE_EP.sh
	- cd EP_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../EP_orig_with_metadata/measurements.csv ./DP_EXECUTE_EP.sh
	- cd EP_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../EP_orig_without_metadata/measurements.csv ./DP_EXECUTE_EP.sh

	