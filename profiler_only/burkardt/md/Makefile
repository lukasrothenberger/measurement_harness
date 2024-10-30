all: cpu

cpu: clean copy_code execute_dynamic_analysis

clean:
	rm -rvf md_*

copy_code:
	cp -r ../../../clean_code/burkardt/md md_orig 

execute_dynamic_analysis:
	rm -rf md_orig/build
	mkdir md_orig/build
	cd md_orig/build && rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake
	cd md_orig/build && cmake -DCMAKE_CXX_COMPILER=discopop_cxx -DCMAKE_CXX_FLAGS="-fopenmp" ..
	cd md_orig/build && make 
	cd md_orig/build && ./prog 3 40 40 
