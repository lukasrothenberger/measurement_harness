all: cpu

cpu: clean copy_code execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata execute_auto_tuner_with_metadata execute_auto_tuner_without_metadata execute_parallel_reference execute_sanity_checker_with_metadata execute_sanity_checker_without_metadata

clean: 
	rm -rvf IS_*

copy_code:
	cp -r ../../../clean_code/NPB-SER IS_orig

execute_hotspot_detection:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make is CLASS=S
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_hotspot_cxx CC=discopop_hotspot_cc make is CLASS=A
	cd IS_orig/bin && ./is.S
	cd IS_orig/bin && ./is.A
	cd IS_orig/bin/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd IS_orig && make clean
	cd IS_orig && DOT_DISCOPOP=$$(pwd)/bin/.discopop CXX=discopop_cxx CC=discopop_cc make is CLASS=S
	cd IS_orig/bin && ./is.S

create_code_version_with_metadata:
	cp -r IS_orig IS_orig_with_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/IS_orig\//\/IS_orig_with_metadata\//g' IS_orig_with_metadata/bin/.discopop/FileMapping.txt

create_code_version_without_metadata:
	cp -r IS_orig IS_orig_without_metadata
# correct the copied FileMapping.txt
	sed -i 's/\/IS_orig\//\/IS_orig_without_metadata\//g' IS_orig_without_metadata/bin/.discopop/FileMapping.txt

execute_pattern_analysis_with_metadata:
	cd IS_orig_with_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd IS_orig_with_metadata/bin/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd IS_orig_without_metadata/bin/.discopop && rm -f profiler/dependency_metadata.txt
	cd IS_orig_without_metadata/bin/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd IS_orig_without_metadata/bin/.discopop && discopop_patch_generator --log info

execute_auto_tuner_with_metadata:
	cd IS_orig_with_metadata && mv DP_COMPILE_IS.sh DP_COMPILE.sh
	cd IS_orig_with_metadata && mv DP_EXECUTE_IS.sh DP_EXECUTE.sh
	cd IS_orig_with_metadata && echo "$$(pwd)"
	cd IS_orig_with_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_auto_tuner_without_metadata:
	cd IS_orig_without_metadata && mv DP_COMPILE_IS.sh DP_COMPILE.sh
	cd IS_orig_without_metadata && mv DP_EXECUTE_IS.sh DP_EXECUTE.sh
	cd IS_orig_without_metadata && echo "$$(pwd)"
	cd IS_orig_without_metadata && discopop_auto_tuner --log info --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_sanity_checker_with_metadata:
	cd IS_orig_with_metadata && mv DP_COMPILE_IS.sh DP_COMPILE.sh
	cd IS_orig_with_metadata && mv DP_EXECUTE_IS.sh DP_EXECUTE.sh
	cd IS_orig_with_metadata && mv DP_COMPILE_SANITIZE_IS.sh DP_COMPILE_SANITIZE.sh
	cd IS_orig_with_metadata && mv DP_EXECUTE_SANITIZE_IS.sh DP_EXECUTE_SANITIZE.sh
	cd IS_orig_with_metadata && mv DP_VALIDATE_IS.sh DP_VALIDATE.sh
	cd IS_orig_with_metadata && echo "$$(pwd)"
	cd IS_orig_with_metadata && discopop_sanity_checker --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_sanity_checker_without_metadata:
	cd IS_orig_without_metadata && mv DP_COMPILE_IS.sh DP_COMPILE.sh
	cd IS_orig_without_metadata && mv DP_EXECUTE_IS.sh DP_EXECUTE.sh
	cd IS_orig_without_metadata && mv DP_COMPILE_SANITIZE_IS.sh DP_COMPILE_SANITIZE.sh
	cd IS_orig_without_metadata && mv DP_EXECUTE_SANITIZE_IS.sh DP_EXECUTE_SANITIZE.sh
	cd IS_orig_without_metadata && mv DP_VALIDATE_IS.sh DP_VALIDATE.sh
	cd IS_orig_without_metadata && echo "$$(pwd)"
	cd IS_orig_without_metadata && discopop_sanity_checker --log debug --project-path $$(pwd) --dot-dp-path $$(pwd)/bin/.discopop

execute_parallel_reference:
	cp -r ../../../clean_code/NPB-OMP IS_OMP
	cd IS_OMP && ./DP_COMPILE_IS.sh
	- cd IS_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../IS_orig_with_metadata/measurements.csv ./DP_EXECUTE_IS.sh
	- cd IS_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../IS_orig_without_metadata/measurements.csv ./DP_EXECUTE_IS.sh

	