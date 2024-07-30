all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf LU_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER LU_SER
	cp -r ../../../clean_code/NPB-OMP LU_OMP

compile_code:
	cd LU_SER && ./DP_COMPILE_LU.sh
	cd LU_OMP && ./DP_COMPILE_LU.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd LU_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_LU.sh
	cd LU_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_LU.sh




