all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf BT_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER BT_SER
	cp -r ../../../clean_code/NPB-OMP BT_OMP

compile_code:
	cd BT_SER && ./DP_COMPILE_BT.sh
	cd BT_OMP && ./DP_COMPILE_BT.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd BT_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_BT.sh
	cd BT_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_BT.sh




