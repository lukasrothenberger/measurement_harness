all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf CG_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER CG_SER
	cp -r ../../../clean_code/NPB-OMP CG_OMP

compile_code:
	cd CG_SER && ./DP_COMPILE_CG.sh
	cd CG_OMP && ./DP_COMPILE_CG.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd CG_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_CG.sh
	cd CG_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_CG.sh




