all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference

clean:
	rm -rvf lulesh_*

copy_code:
	
	cp -r ../../clean_code/LULESH_SEQ lulesh_orig 

execute_hotspot_detection:

	cd lulesh_orig && mkdir build
	cd lulesh_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_hotspot_cxx -DWITH_MPI=Off -DWITH_OPENMP=Off ..
	cd lulesh_orig/build && make 
	
	cd lulesh_orig/build && ./lulesh2.0 -s 5
	cd lulesh_orig/build && ./lulesh2.0 -s 8
	cd lulesh_orig/build && ./lulesh2.0 -s 10
	cd lulesh_orig/build/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd lulesh_orig/build && rm -r CMakeCache.txt CMakeFiles cmake_install.cmake
	cd lulesh_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_cxx -DWITH_MPI=Off -DWITH_OPENMP=Off ..
	cd lulesh_orig/build && make 
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
	cd lulesh_orig_with_metadata/build/.discopop && discopop_explorer --enable-patterns doall,reduction --enable-statistics

execute_pattern_analysis_without_metadata:
	cd lulesh_orig_without_metadata/build/.discopop && rm -f profiler/dependency_metadata.txt
	cd lulesh_orig_without_metadata/build/.discopop && discopop_explorer --enable-patterns doall,reduction --enable-statistics

execute_auto_tuner_with_metadata:
	cd lulesh_orig_with_metadata && echo "$$(pwd)"
	cd lulesh_orig_with_metadata && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/build/.discopop --skip-cleanup

execute_auto_tuner_without_metadata:
	cd lulesh_orig_without_metadata && echo "$$(pwd)"
	cd lulesh_orig_without_metadata && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/build/.discopop
	
execute_parallel_reference:
	cp -r ../../clean_code/LULESH_PAR lulesh_PAR
	cd lulesh_PAR && ./DP_COMPILE.sh
	cd lulesh_PAR && /usr/bin/time --append --format="REF; %e; %x;" -o ../lulesh_orig_with_metadata/measurements.csv ./DP_EXECUTE.sh
	cd lulesh_PAR && /usr/bin/time --append --format="REF; %e; %x;" -o ../lulesh_orig_without_metadata/measurements.csv ./DP_EXECUTE.sh
