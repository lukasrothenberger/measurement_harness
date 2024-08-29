all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata

clean: 
	rm -rvf FT_*

copy_code:
	cp -r ../../../clean_code/NPB-SER FT_orig

execute_hotspot_detection:
	cd FT_orig && make clean
	cd FT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make ft CLASS=S
	cd FT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make ft CLASS=A
	cd FT_orig/bin && ./ft.S
	cd FT_orig/bin && ./ft.A
	cd FT_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd FT_orig && make clean
	cd FT_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make ft CLASS=S
	cd FT_orig/bin && ./ft.S

create_code_version_with_metadata:
	cp -r FT_orig FT_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/FT_orig\//\/FT_orig_with_metadata\//g' FT_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r FT_orig FT_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/FT_orig\//\/FT_orig_without_metadata\//g' FT_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd FT_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd FT_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd FT_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd FT_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd FT_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info
