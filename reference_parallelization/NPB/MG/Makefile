all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf MG_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER MG_SER
	cp -r ../../../clean_code/NPB-OMP MG_OMP

compile_code:
	cd MG_SER && ./DP_COMPILE_MG.sh
	cd MG_OMP && ./DP_COMPILE_MG.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd MG_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_MG.sh
	cd MG_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_MG.sh




