all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata

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
