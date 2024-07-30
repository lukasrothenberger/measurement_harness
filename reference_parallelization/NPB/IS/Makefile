all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf IS_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER IS_SER
	cp -r ../../../clean_code/NPB-OMP IS_OMP

compile_code:
	cd IS_SER && ./DP_COMPILE_IS.sh
	cd IS_OMP && ./DP_COMPILE_IS.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd IS_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_IS.sh
	cd IS_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_IS.sh




