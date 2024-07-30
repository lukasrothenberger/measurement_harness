all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner execute_parallel_reference

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
	cd md_orig/build/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd md_orig/build/.discopop && discopop_patch_generator 

execute_auto_tuner:
	cd md_orig && echo "$$(pwd)"
	cd md_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/build/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/burkardt/md_parallel md_PAR
	cd md_PAR && ./DP_COMPILE.sh
	cd md_PAR && /usr/bin/time --append --format="REF; %e; %x;" -o ../md_orig/measurements.csv ./DP_EXECUTE.sh