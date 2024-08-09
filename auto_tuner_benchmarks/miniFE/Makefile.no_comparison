MINIFE_DIR=$$(pwd)

all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis execute_auto_tuner execute_parallel_reference

clean:
	rm -rvf miniFE_*

copy_code:
	cp -r ../../clean_code/miniFE/ref miniFE_orig 

execute_hotspot_detection:
	echo "MINIFE_DIR: ${MINIFE_DIR}"
	cd miniFE_orig/src && DP_PROJECT_ROOT_DIR=${MINIFE_DIR} make -f Makefile.discopop.hotspots
	cd miniFE_orig/src && ./miniFE.x --nx 5 --ny 5 --nz 5
	cd miniFE_orig/src && ./miniFE.x --nx 25 --ny 25 --nz 25
	cd miniFE_orig/src && ./miniFE.x --nx 35 --ny 35 --nz 35
	cd miniFE_orig/src/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd miniFE_orig/src && make clean
	cd miniFE_orig/src && DP_PROJECT_ROOT_DIR=${MINIFE_DIR} make -f Makefile.discopop.patterns
	cd miniFE_orig/src && ./miniFE.x --nx 10 --ny 10 --nz 10 
	cd miniFE_orig/src/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd miniFE_orig/src/.discopop && discopop_patch_generator 

execute_auto_tuner:
	cd miniFE_orig && echo "$$(pwd)"
	cd miniFE_orig && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/src/.discopop

execute_parallel_reference:
	cp -r ../../clean_code/miniFE/openmp miniFE_openmp
	cd miniFE_openmp && ./DP_COMPILE.sh
	- cd miniFE_openmp && /usr/bin/time --append --format="REF; %e; %x;" -o ../miniFE_orig/measurements.csv ./DP_EXECUTE.sh