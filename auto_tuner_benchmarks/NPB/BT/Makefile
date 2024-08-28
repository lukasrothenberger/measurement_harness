all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean: 
	rm -rvf BT_*

copy_code:
	cp -r ../../../clean_code/NPB-SER BT_orig

execute_hotspot_detection:
	cd BT_orig && make clean
	cd BT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make bt CLASS=S
	cd BT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make bt CLASS=A
	cd BT_orig/bin && ./bt.S
	cd BT_orig/bin && ./bt.A
	cd BT_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd BT_orig && make clean
	cd BT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make bt CLASS=S
	cd BT_orig/bin && ./bt.S

create_code_version_with_metadata:
	cp -r BT_orig BT_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/BT_orig\//\/BT_orig_with_metadata\//g' BT_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r BT_orig BT_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/BT_orig\//\/BT_orig_without_metadata\//g' BT_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd BT_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd BT_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd BT_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd BT_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd BT_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd BT_orig_with_metadata && mv DP_COMPILE_BT.sh DP_COMPILE.sh
	cd BT_orig_with_metadata && mv DP_EXECUTE_BT.sh DP_EXECUTE.sh
	cd BT_orig_with_metadata && echo "$$(pwd)"
	cd BT_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd BT_orig_without_metadata && mv DP_COMPILE_BT.sh DP_COMPILE.sh
	cd BT_orig_without_metadata && mv DP_EXECUTE_BT.sh DP_EXECUTE.sh
	cd BT_orig_without_metadata && echo "$$(pwd)"
	cd BT_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP BT_OMP
	cd BT_OMP && ./DP_COMPILE_BT.sh
	- cd BT_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../BT_orig_with_metadata/measurements.csv ./DP_EXECUTE_BT.sh
	- cd BT_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../BT_orig_without_metadata/measurements.csv ./DP_EXECUTE_BT.sh

	