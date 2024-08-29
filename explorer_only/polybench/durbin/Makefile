BENCHMARK_NAME=$$(basename ${CURDIR})


all: cpu

cpu: clean copy_code get_reference_output execute_hotspot_detection execute_dynamic_analysis create_code_version_with_metadata create_code_version_without_metadata execute_pattern_analysis_with_metadata execute_pattern_analysis_without_metadata

clean:
	rm -rvf orig*

copy_code:
	cp -r ../../../clean_code/polybench_seq orig 

get_reference_output:
	cd orig && make -f Makefile.discopop compile_${BENCHMARK_NAME}
	cd orig && ./${BENCHMARK_NAME}_base_dump_arrays 2> reference_output.txt

execute_hotspot_detection:
	cd orig && CC=discopop_hotspot_cc make -f Makefile.discopop compile_${BENCHMARK_NAME}
	cd orig && ./${BENCHMARK_NAME}_base
	cd orig/.discopop && hotspot_analyzer

execute_dynamic_analysis:
	cd orig && make -f Makefile.discopop instrument_${BENCHMARK_NAME}
	cd orig && ./${BENCHMARK_NAME}_base

create_code_version_with_metadata:
	cp -r orig orig_with_metadata
# clean executable
	rm -f orig_with_metadata/${BENCHMARK_NAME}_base
# correct the copied FileMapping.txt
	sed -i 's/\/orig\//\/orig_with_metadata\//g' orig_with_metadata/.discopop/FileMapping.txt
# prepare DP_COMPILE.sh
	echo ${BENCHMARK_NAME} >> orig_with_metadata/DP_COMPILE.sh
# prepare DP_EXECUTE.sh
	echo "./${BENCHMARK_NAME}_base" >> orig_with_metadata/DP_EXECUTE.sh
# prepare DP_VALIDATE.sh
	echo "./${BENCHMARK_NAME}_base_dump_arrays 2> test_output.txt && diff reference_output.txt test_output.txt" >> orig_with_metadata/DP_VALIDATE.sh

create_code_version_without_metadata:
	cp -r orig orig_without_metadata
# clean executable
	rm -f orig_without_metadata/${BENCHMARK_NAME}_base
# correct the copied FileMapping.txt
	sed -i 's/\/orig\//\/orig_without_metadata\//g' orig_without_metadata/.discopop/FileMapping.txt
# prepare DP_COMPILE.sh
	echo ${BENCHMARK_NAME} >> orig_without_metadata/DP_COMPILE.sh
# prepare DP_EXECUTE.sh
	echo "./${BENCHMARK_NAME}_base" >> orig_without_metadata/DP_EXECUTE.sh
# prepare DP_VALIDATE.sh
	echo "./${BENCHMARK_NAME}_base_dump_arrays 2> test_output.txt && diff reference_output.txt test_output.txt" >> orig_without_metadata/DP_VALIDATE.sh
# modify dynamic dependency file to reflect legacy discopop output
	cd $$(discopop_config_provider -s)/discopop_library/Compatibility/LegacyDiscoPoP && python -m GEPDependencyRemover --log info --dyndep-file-path ${CURDIR}/orig_without_metadata/.discopop/profiler/dynamic_dependencies.txt

execute_pattern_analysis_with_metadata:
	cd orig_with_metadata/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd orig_with_metadata/.discopop && discopop_patch_generator --log info

execute_pattern_analysis_without_metadata:
	cd orig_without_metadata/.discopop && rm -f profiler/dependency_metadata.txt
	cd orig_without_metadata/.discopop && discopop_explorer --enable-patterns doall,reduction
	cd orig_without_metadata/.discopop && discopop_patch_generator

