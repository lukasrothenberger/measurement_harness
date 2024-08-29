all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata

clean: 
	rm -rvf IS_*

copy_code:
	cp -r ../../../clean_code/NPB-SER IS_orig

execute_hotspot_detection:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make is CLASS=S
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make is CLASS=A
	cd IS_orig/bin && ./is.S
	cd IS_orig/bin && ./is.A
	cd IS_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make is CLASS=S
	cd IS_orig/bin && ./is.S

create_code_version_with_metadata:
	cp -r IS_orig IS_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/IS_orig\//\/IS_orig_with_metadata\//g' IS_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r IS_orig IS_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/IS_orig\//\/IS_orig_without_metadata\//g' IS_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd IS_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd IS_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd IS_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd IS_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd IS_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

