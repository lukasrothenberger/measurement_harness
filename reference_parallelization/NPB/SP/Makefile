all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf SP_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER SP_SER
	cp -r ../../../clean_code/NPB-OMP SP_OMP

compile_code:
	cd SP_SER && ./DP_COMPILE_SP.sh
	cd SP_OMP && ./DP_COMPILE_SP.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd SP_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_SP.sh
	cd SP_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_SP.sh




