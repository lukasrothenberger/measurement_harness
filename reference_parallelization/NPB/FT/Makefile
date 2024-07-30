all: cpu

cpu: clean copy_code compile_code get_measurements

clean: 
	rm -rvf FT_*
	rm -f dp_execute_out.txt
	rm -f measurements.csv

copy_code:
	cp -r ../../../clean_code/NPB-SER FT_SER
	cp -r ../../../clean_code/NPB-OMP FT_OMP

compile_code:
	cd FT_SER && ./DP_COMPILE_FT.sh
	cd FT_OMP && ./DP_COMPILE_FT.sh

get_measurements:
	echo "ID; time; return_code;" > measurements.csv
	cd FT_SER && /usr/bin/time --append --format="SER; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_FT.sh
	cd FT_OMP && /usr/bin/time --append --format="REF; %e; %x;" -o ../measurements.csv ./DP_EXECUTE_FT.sh




