all: cpu

cpu: clean copy_code execute_dynamic_analysis

clean:
	rm -rvf miniFE_*

copy_code:
	cp -r ../../clean_code/miniFE/ref miniFE_orig 

execute_dynamic_analysis:
	cd miniFE_orig/src && make clean
	cd miniFE_orig/src && DP_PROJECT_ROOT_DIR=${MINIFE_DIR} make -f Makefile.discopop.patterns
	cd miniFE_orig/src && ./miniFE.x --nx 10 --ny 10 --nz 10 