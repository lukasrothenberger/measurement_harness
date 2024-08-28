all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean: 
	rm -rvf SP_*

copy_code:
	cp -r ../../../clean_code/NPB-SER SP_orig

execute_hotspot_detection:
	cd SP_orig && make clean
	cd SP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make sp CLASS=S
	cd SP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make sp CLASS=A
	cd SP_orig/bin && ./sp.S
	cd SP_orig/bin && ./sp.A
	cd SP_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd SP_orig && make clean
	cd SP_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make sp CLASS=S
	cd SP_orig/bin && ./sp.S

create_code_version_with_metadata:
	cp -r SP_orig SP_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/SP_orig\//\/SP_orig_with_metadata\//g' SP_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r SP_orig SP_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/SP_orig\//\/SP_orig_without_metadata\//g' SP_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd SP_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd SP_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd SP_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd SP_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd SP_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd SP_orig_with_metadata && mv DP_COMPILE_SP.sh DP_COMPILE.sh
	cd SP_orig_with_metadata && mv DP_EXECUTE_SP.sh DP_EXECUTE.sh
	cd SP_orig_with_metadata && echo "$$(pwd)"
	cd SP_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd SP_orig_without_metadata && mv DP_COMPILE_SP.sh DP_COMPILE.sh
	cd SP_orig_without_metadata && mv DP_EXECUTE_SP.sh DP_EXECUTE.sh
	cd SP_orig_without_metadata && echo "$$(pwd)"
	cd SP_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP SP_OMP
	cd SP_OMP && ./DP_COMPILE_SP.sh
	- cd SP_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../SP_orig_with_metadata/measurements.csv ./DP_EXECUTE_SP.sh
	- cd SP_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../SP_orig_without_metadata/measurements.csv ./DP_EXECUTE_SP.sh

	