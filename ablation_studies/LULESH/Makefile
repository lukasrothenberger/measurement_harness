LULESH_PROBLEM_SIZE=20  # default 30

all: clean prepare seq par

prepare:
	echo "Name;Time;Code;" > measurements.csv

seq:
	cd LULESH_SEQ && rm -rf build
	cd LULESH_SEQ && mkdir build
	cd LULESH_SEQ/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_SEQ/build && /usr/bin/time --append --output=../../measurements.csv --format="SEQ;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

par:
	NAME=PAR
	cd LULESH_PAR && rm -rf build
	cd LULESH_PAR && mkdir build
	cd LULESH_PAR/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=On && make -j
	cd LULESH_PAR/build && /usr/bin/time --append --output=../../measurements.csv --format="PAR;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

clean:
	rm -f measurements.csv


