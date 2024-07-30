all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf EP_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER EP_SER
	cp -r ../../../clean_code/NPB-OMP EP_OMP

compile_code:
	cd EP_SER && ./DP_COMPILE_EP.sh
	cd EP_OMP && ./DP_COMPILE_EP.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd EP_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_EP.sh
	cd EP_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_EP.sh




