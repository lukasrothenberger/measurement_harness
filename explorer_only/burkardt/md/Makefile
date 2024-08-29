all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata

clean:
	rm -rvf md_*

copy_code:
	cp -r ../../../clean_code/burkardt/md md_orig 

execute_hotspot_detection:
	cd md_orig && mkdir build
	cd md_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_hotspot_cxx -DCMAKE_CXX_FLAGS="-fopenmp" ..
	cd md_orig/build && make 
	cd md_orig/build && ./prog 3 40 40
	cd md_orig/build && ./prog 3 300 100
	cd md_orig/build/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd md_orig/build && rm -r CMakeCache.txt CMakeFiles cmake_install.cmake
	cd md_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_cxx -DCMAKE_CXX_FLAGS="-fopenmp" ..
	cd md_orig/build && make 
	cd md_orig/build && ./prog 3 40 40 

create_code_version_with_metadata:
	cp -r md_orig md_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/md_orig\//\/md_orig_with_metadata\//g' md_orig_with_metadata/build/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r md_orig md_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/md_orig\//\/md_orig_without_metadata\//g' md_orig_without_metadata/build/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd md_orig_with_metadata/build/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd md_orig_with_metadata/build/.discopop && discopop_patch_generator  --log info

execute_pattern_analysis_without_metadata:
	cd md_orig_without_metadata/build/.discopop && rm -f profiler/dependency_metadata.txt
	cd md_orig_without_metadata/build/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd md_orig_without_metadata/build/.discopop && discopop_patch_generator