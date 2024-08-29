all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis

clean: 
	rm -rvf cfd_*

copy_code:
	cp -r ../../../clean_code/rodinia_3.1 cfd_orig

execute_hotspot_detection:
	cd cfd_orig/openmp/cfd && CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make euler3d_cpu
	cd cfd_orig/openmp/cfd && ./euler3d_cpu ../../../../../../data/rodinia-3.1-data/data/cfd/fvcorr.domn.097K
#	cd cfd_orig/openmp/cfd && ./euler3d_cpu ../../../../../../data/rodinia-3.1-data/data/cfd/fvcorr.domn.193K
	cd cfd_orig/openmp/cfd/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd cfd_orig/openmp/cfd && make clean
	cd cfd_orig/openmp/cfd && DOT_DISCOPOP=$$(pwd)/.discopop CXX=discopop_cxx CC=discopop_cc make euler3d_cpu
	cd cfd_orig/openmp/cfd && ./euler3d_cpu ../../../../../../data/rodinia-3.1-data/data/cfd/fvcorr.domn.097K
	cd cfd_orig/openmp/cfd/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd cfd_orig/openmp/cfd/.discopop && discopop_patch_generator
