BENCHMARK_NAME=$$(basename ${CURDIR})
CC=clang
CFLAGS=-fopenmp
CFLAGS_SEQUENTIAL=

clean:
	cd orig_with_metadata_* && rm -f ${BENCHMARK_NAME}_*
	cd orig_without_metadata_* && rm -f ${BENCHMARK_NAME}_*
	rm -rf sequential
	rm -f measurements_sequential.csv
	rm -f measurements_vanilla.csv
	rm -f measurements_extended.csv
	rm -f measurements_inputs.csv
	
veryclean: clean
	rm -rf orig_with_metadata_*
	rm -rf orig_without_metadata_*

get_code:
	cp -r ../../../auto_tuner_benchmarks/polybench/atax/orig_with_metadata_* . 
	cp -r ../../../auto_tuner_benchmarks/polybench/atax/orig_without_metadata_* . 

prepare:
	cp -r orig_with_metadata_* sequential
	echo "Input;" >> measurements_inputs.csv
	echo "Sequential;" >> measurements_sequential.csv
	echo "Vanilla;" >> measurements_vanilla.csv
	echo "Extended;" >> measurements_extended.csv

compile: clean prepare
	cd orig_with_metadata_* && make -f Makefile.discopop CC=${CC} CFLAGS=${CFLAGS} compile_${BENCHMARK_NAME}
	cd orig_without_metadata_* && make -f Makefile.discopop CC=${CC} CFLAGS=${CFLAGS} compile_${BENCHMARK_NAME}
	cd sequential && make -f Makefile.discopop CC=${CC} CFLAGS=${CFLAGS_SEQUENTIAL} compile_${BENCHMARK_NAME}

execute:
	for input in mini small standard large ; do \
		echo "$$input" >> measurements_inputs.csv ; \
		/usr/bin/time --append -o measurements_sequential.csv --format=";%e;" sequential/${BENCHMARK_NAME}_base_$$input ; \
		/usr/bin/time --append -o measurements_vanilla.csv --format="%e;" orig_without_metadata_*/${BENCHMARK_NAME}_base_$$input ; \
		/usr/bin/time --append -o measurements_extended.csv --format="%e;" orig_with_metadata_*/${BENCHMARK_NAME}_base_$$input  ; \
    done

# 	correct formatting for google slides
#	sed -i -e 's/\./,/g' measurements_sequential.csv
#	sed -i -e 's/\./,/g' measurements_vanilla.csv
#	sed -i -e 's/\./,/g' measurements_extended.csv

plot:
	python ../../plot.py $$(pwd)
