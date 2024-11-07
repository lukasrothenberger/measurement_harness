mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference execute_sanity_checker_with_metadata execute_sanity_checker_without_metadata

clean: 
	rm -rvf ${current_dir}_*

copy_code:
	cp -r ../../../clean_code/rodinia_3.1 ${current_dir}_orig

execute_hotspot_detection:
	cd ${current_dir}_orig && DOT_DISCOPOP=$$(pwd)/.discopop DP_PROJECT_ROOT_DIR=$$(pwd) CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc ./DP_COMPILE_${current_dir}.sh 
	cd ${current_dir}_orig && DOT_DISCOPOP=$$(pwd)/.discopop DP_PROJECT_ROOT_DIR=$$(pwd) ./DP_EXECUTE_${current_dir}_DP.sh
	cd ${current_dir}_orig && DOT_DISCOPOP=$$(pwd)/.discopop DP_PROJECT_ROOT_DIR=$$(pwd) ./DP_EXECUTE_${current_dir}.sh
	cd ${current_dir}_orig/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd ${current_dir}_orig && make clean
	cd ${current_dir}_orig && DP_PROJECT_ROOT_DIR=$$(pwd) DOT_DISCOPOP=$$(pwd)/.discopop CXX=discopop_cxx CC=discopop_cc ./DP_COMPILE_${current_dir}.sh
	cd ${current_dir}_orig && DOT_DISCOPOP=$$(pwd)/.discopop ./DP_EXECUTE_${current_dir}_DP.sh
#	cd ${current_dir}_orig/.discopop && discopop_explorer --enable-patterns doall,reduction
#	cd ${current_dir}_orig/.discopop && discopop_patch_generator

#execute_auto_tuner:
#	cd ${current_dir}_orig && cp DP_COMPILE_${current_dir}.sh DP_COMPILE.sh
#	cd ${current_dir}_orig && cp DP_EXECUTE_${current_dir}.sh DP_EXECUTE.sh
#	cd ${current_dir}_orig && discopop_auto_tuner --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/.discopop --skip-cleanup


create_code_version_with_metadata:
	cp -r ${current_dir}_orig ${current_dir}_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/${current_dir}_orig\//\/${current_dir}_orig_with_metadata\//g' ${current_dir}_orig_with_metadata/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r ${current_dir}_orig ${current_dir}_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/${current_dir}_orig\//\/${current_dir}_orig_without_metadata\//g' ${current_dir}_orig_without_metadata/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd ${current_dir}_orig_with_metadata/.discopop && discopop_explorer --enable-patterns doall,reduction --enable-statistics

execute_pattern_analysis_without_metadata:
	cd ${current_dir}_orig_without_metadata/.discopop && rm -f profiler/dependency_metadata.txt
	cd ${current_dir}_orig_without_metadata/.discopop && discopop_explorer --enable-patterns doall,reduction --enable-statistics

execute_auto_tuner_with_metadata:
	cd ${current_dir}_orig_with_metadata && cp DP_COMPILE_${current_dir}.sh DP_COMPILE.sh
	cd ${current_dir}_orig_with_metadata && cp DP_EXECUTE_${current_dir}.sh DP_EXECUTE.sh
	cd ${current_dir}_orig_with_metadata && echo "$$(pwd)"
	cd ${current_dir}_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/.discopop

execute_auto_tuner_without_metadata:
	cd ${current_dir}_orig_without_metadata && cp DP_COMPILE_${current_dir}.sh DP_COMPILE.sh
	cd ${current_dir}_orig_without_metadata && cp DP_EXECUTE_${current_dir}.sh DP_EXECUTE.sh
	cd ${current_dir}_orig_without_metadata && echo "$$(pwd)"
	cd ${current_dir}_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/.discopop

execute_sanity_checker_with_metadata:
	cd ${current_dir}_orig_with_metadata && cp DP_COMPILE_SANITIZE_${current_dir}.sh DP_COMPILE_SANITIZE.sh
	cd ${current_dir}_orig_with_metadata && cp DP_EXECUTE_${current_dir}_DP.sh DP_EXECUTE_SANITIZE.sh
	cd ${current_dir}_orig_with_metadata && echo "$$(pwd)"
	cd ${current_dir}_orig_with_metadata && discopop_sanity_checker --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/.discopop

execute_sanity_checker_without_metadata:
	cd ${current_dir}_orig_without_metadata && cp DP_COMPILE_SANITIZE_${current_dir}.sh DP_COMPILE_SANITIZE.sh
	cd ${current_dir}_orig_without_metadata && cp DP_EXECUTE_${current_dir}_DP.sh DP_EXECUTE_SANITIZE.sh
	cd ${current_dir}_orig_without_metadata && echo "$$(pwd)"
	cd ${current_dir}_orig_without_metadata && discopop_sanity_checker --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/rodinia_3.1-OMP ${current_dir}_OMP
	cp ${current_dir}_orig/DP_COMPILE_${current_dir}.sh ${current_dir}_OMP/DP_COMPILE_${current_dir}.sh
	cp ${current_dir}_orig/DP_EXECUTE_${current_dir}.sh ${current_dir}_OMP/DP_EXECUTE_${current_dir}.sh
	cd ${current_dir}_OMP && ./DP_COMPILE_${current_dir}.sh
	- cd ${current_dir}_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../${current_dir}_orig_with_metadata/measurements.csv ./DP_EXECUTE_${current_dir}.sh
	- cd ${current_dir}_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../${current_dir}_orig_without_metadata/measurements.csv ./DP_EXECUTE_${current_dir}.sh

	
