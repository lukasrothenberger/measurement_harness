all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean: 
	rm -rvf LU_*

copy_code:
	cp -r ../../../clean_code/NPB-SER LU_orig

execute_hotspot_detection:
	cd LU_orig && make clean
	cd LU_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make lu CLASS=S
	cd LU_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make lu CLASS=A
	cd LU_orig/bin && ./lu.S
	cd LU_orig/bin && ./lu.A
	cd LU_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd LU_orig && make clean
	cd LU_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make lu CLASS=S
	cd LU_orig/bin && ./lu.S

create_code_version_with_metadata:
	cp -r LU_orig LU_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/LU_orig\//\/LU_orig_with_metadata\//g' LU_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r LU_orig LU_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/LU_orig\//\/LU_orig_without_metadata\//g' LU_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd LU_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd LU_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd LU_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd LU_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd LU_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd LU_orig_with_metadata && mv DP_COMPILE_LU.sh DP_COMPILE.sh
	cd LU_orig_with_metadata && mv DP_EXECUTE_LU.sh DP_EXECUTE.sh
	cd LU_orig_with_metadata && echo "$$(pwd)"
	cd LU_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd LU_orig_without_metadata && mv DP_COMPILE_LU.sh DP_COMPILE.sh
	cd LU_orig_without_metadata && mv DP_EXECUTE_LU.sh DP_EXECUTE.sh
	cd LU_orig_without_metadata && echo "$$(pwd)"
	cd LU_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP LU_OMP
	cd LU_OMP && ./DP_COMPILE_LU.sh
	- cd LU_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../LU_orig_with_metadata/measurements.csv ./DP_EXECUTE_LU.sh
	- cd LU_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../LU_orig_without_metadata/measurements.csv ./DP_EXECUTE_LU.sh

	