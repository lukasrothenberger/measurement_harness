COMMAND=code/prog

cpu: clean setup_env measure_baseline

measure_baseline: reset_env
	mkdir -p logs/baseline
	cd code && cmake . && make
	/usr/bin/time --format="baseline;%e;%x;" --append --output=measurements.csv ${COMMAND} 1>> logs/baseline/stdout.txt 2>> logs/baseline/stderr.txt

setup_env:
	echo "ID; time; return_code;" > measurements.csv

reset_env:
	cp -r ../../../clean_code/daxpy/50k code


clean: 
	rm -rvf code
	rm -rvf dp_folder
	rm -rvf logs
	rm -rvf measurements.csv




