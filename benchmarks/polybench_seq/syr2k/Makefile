CC=clang
CXX=clang++
CPU_CC_FLAGS="-fopenmp"
CPU_CXX_FLAGS="-fopenmp"
GPU_CC_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
GPU_CXX_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
DP_OPTIMIZER_FLAGS=--doall-microbench-file ../../../../configuration/doall_1.json --reduction-microbench-file ../../../../configuration/reduction.json
DP_PATCH_GENERATOR_FLAGS=--log INFO #--only-maximum-id-pattern

########## CPU + GPU
all: clean setup_env cpu_prepare_code cpu_compile cpu_measure

########## CPU
cpu: clean setup_env cpu_prepare_code cpu_compile cpu_measure

cpu_prepare_code: 
	make prepare_sequential
	make cpu_get_discopop_patterns
	make prepare_single_suggestions

cpu_compile:
	make compile_sequential
	make compile_single_suggestions

cpu_measure:
	make measure_sequential
	make measure_single_suggestions

cpu_cleanup:
	rm -rvf code_*


##### DISCOPOP 
cpu_get_discopop_patterns:
	make reset_env
	cd code && make -f Makefile.discopop instrument_syr2k
	cd code && ./syr2k_base
	ls code/.discopop
	cp -r code/.discopop/* clean_dp_folder
	ls -a clean_dp_folder
	cd clean_dp_folder && discopop_explorer --enable-patterns=reduction,doall
	cd clean_dp_folder && discopop_optimizer -v ${DP_OPTIMIZER_FLAGS} --system-configuration ../../../../configuration/cpu_only_system_configuration.json
	cd clean_dp_folder && discopop_patch_generator -a optimizer/patterns.json ${DP_PATCH_GENERATOR_FLAGS}

##### SEQUENTIAL
prepare_sequential:
	make reset_env
	mv code code_sequential
	mkdir -p logs/sequential
compile_sequential:
	-cd code_sequential && CC=${CC} CFLAGS=${CPU_CC_FLAGS} make -f Makefile.discopop compile_syr2k
measure_sequential:
	-/usr/bin/time --format="sequential;%e;%x;" --append --output=measurements.csv code_sequential/syr2k_base 1>> logs/sequential/stdout.txt 2>> logs/sequential/stderr.txt



##### SINGLE SUGGESTIONS
SUGGESTIONS_DIR=./clean_dp_folder/patch_generator
prepare_single_suggestions: $(SUGGESTIONS_DIR)/*
	for f in $^; do \
		basename $${f}; \
		rm -rvf code_$$(basename $${f}); \
		make reset_env; \
		ls; \
		pwd; \
		cd dp_folder && discopop_patch_applicator -v -a $$(basename $${f}); \
		cd .. ;\
		mv code code_$$(basename $${f}); \
		mkdir -p logs/$$(basename $${f}); \
	done
compile_single_suggestions: $(SUGGESTIONS_DIR)/*
	for f in $^; do \
		cd code_$$(basename $${f}) && CC=${CC} CFLAGS=${CPU_CC_FLAGS} make -f Makefile.discopop compile_syr2k ; \
		cd .. ; \
	done
measure_single_suggestions: $(SUGGESTIONS_DIR)/*
	for f in $^; do \
		/usr/bin/time --format="$$(basename $${f});%e;%x;" --append --output=measurements.csv timeout 300 code_$$(basename $${f})/syr2k_base 1>> logs/$$(basename $${f})/stdout.txt 2>> logs/$$(basename $${f})/stderr.txt ; \
	done


setup_env:
	mkdir clean_dp_folder

reset_env:
	echo "RESET ENV"
	rm -rvf code
	cp -r ../../../clean_code/polybench_seq code
	rm -rvf dp_folder
	mkdir -p clean_dp_folder
	cp -r clean_dp_folder dp_folder

clean:
	rm -rvf code*
	rm -rvf clean_dp_folder
	rm -rvf dp_folder
	rm -rvf measurements.csv
	rm -rvf logs
	make cpu_cleanup


