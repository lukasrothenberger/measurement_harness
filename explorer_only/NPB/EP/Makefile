all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata 
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
