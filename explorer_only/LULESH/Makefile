all: cpu

cpu: clean copy_code prepare execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata 

clean:
	rm -rvf lulesh_*

copy_code:
	
	cp -r ../../clean_code/LULESH_SEQ lulesh_orig 

prepare:
	cd lulesh_orig && mkdir build

execute_hotspot_detection:
	cd lulesh_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_hotspot_cxx -DWITH_MPI=Off -DWITH_OPENMP=Off ..
	cd lulesh_orig/build && make 
	
	cd lulesh_orig/build && ./lulesh2.0 -s 5
	cd lulesh_orig/build && ./lulesh2.0 -s 8
	cd lulesh_orig/build && ./lulesh2.0 -s 10
	cd lulesh_orig/build/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd lulesh_orig/build && rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake
	cd lulesh_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_cxx -DWITH_MPI=Off -DWITH_OPENMP=Off ..
	cd lulesh_orig/build && DP_PROJECT_ROOT_DIR=/home/lukas/git/benchmark_harness/explorer_only/LULESH make 
	cd lulesh_orig/build && ./lulesh2.0 -s 3

create_code_version_with_metadata:
	cp -r lulesh_orig lulesh_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/lulesh_orig\//\/lulesh_orig_with_metadata\//g' lulesh_orig_with_metadata/build/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r lulesh_orig lulesh_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/lulesh_orig\//\/lulesh_orig_without_metadata\//g' lulesh_orig_without_metadata/build/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd lulesh_orig_with_metadata/build/.discopop && discopop_explorer --enable-patterns doall,reduction --disable-statistics --profiling profile.txt
	cd lulesh_orig_with_metadata/build/.discopop && discopop_patch_generator  --log info

execute_pattern_analysis_without_metadata:
	cd lulesh_orig_without_metadata/build/.discopop && rm -f profiler/dependency_metadata.txt
	cd lulesh_orig_without_metadata/build/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd lulesh_orig_without_metadata/build/.discopop && discopop_patch_generator