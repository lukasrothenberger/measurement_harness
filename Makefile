all:
	cd benchmarks && make all

cpu:
	cd benchmarks && make cpu

gpu:
	cd benchmarks && make gpu

clean:
	cd benchmarks && make clean
	cd auto_tuner_benchmarks && make clean
	cd reference_parallelization && make clean
	cd auto_tuner_benchmarks && make clean
	cd profiler_only && make clean
	rm -f measurement_report.pdf
