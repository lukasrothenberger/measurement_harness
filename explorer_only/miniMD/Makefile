miniMD_DIR=${CURDIR}

all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata

clean:
	rm -rvf miniMD_*

copy_code:
	cp -r ../../clean_code/miniMD/ref miniMD_orig 

execute_hotspot_detection:
	echo "miniMD_DIR: ${miniMD_DIR}"
	cd miniMD_orig && DP_PROJECT_ROOT_DIR=${miniMD_DIR} DOT_DISCOPOP=${miniMD_DIR}/miniMD_orig/.discopop make -f Makefile.discopop.hotspots
	cd miniMD_orig && DOT_DISCOPOP=${miniMD_DIR}/miniMD_orig/.discopop ./miniMD -nx 8 -ny 8 -nz 8
	cd miniMD_orig && DOT_DISCOPOP=${miniMD_DIR}/miniMD_orig/.discopop ./miniMD -nx 16 -ny 16 -nz 16
	cd miniMD_orig && DOT_DISCOPOP=${miniMD_DIR}/miniMD_orig/.discopop ./miniMD -nx 24 -ny 24 -nz 24
	cd miniMD_orig/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd miniMD_orig && make -f Makefile.discopop.patterns clean
	cd miniMD_orig && DP_PROJECT_ROOT_DIR=${miniMD_DIR} DOT_DISCOPOP=${miniMD_DIR}/miniMD_orig/.discopop make -f Makefile.discopop.patterns
	cd miniMD_orig && DOT_DISCOPOP=${miniMD_DIR}/miniMD_orig/.discopop ./miniMD -nx 8 -ny 8 -nz 8 -n 20

create_code_version_with_metadata:
	cp -r miniMD_orig miniMD_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/miniMD_orig\//\/miniMD_orig_with_metadata\//g' miniMD_orig_with_metadata/.discopop/FileMapping.txt
	sed -i 's/\/Obj_default\//\//g' miniMD_orig_with_metadata/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r miniMD_orig miniMD_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/miniMD_orig\//\/miniMD_orig_without_metadata\//g' miniMD_orig_without_metadata/.discopop/FileMapping.txt
	sed -i 's/\/Obj_default\//\//g' miniMD_orig_without_metadata/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd miniMD_orig_with_metadata/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd miniMD_orig_with_metadata/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd miniMD_orig_without_metadata/.discopop && rm -f profiler/dependency_metadata.txt
	cd miniMD_orig_without_metadata/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd miniMD_orig_without_metadata/.discopop && discopop_patch_generator --log info
