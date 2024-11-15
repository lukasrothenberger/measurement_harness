LULESH_PROBLEM_SIZE=25  # default 30

all: clean prepare seq par codes

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
	rm -rf $$(find -type d -name build)

codes: 1 2 3 4 5 6 7 8 9 10 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 34 35 40 41 43

1:
	cd LULESH_1 && rm -rf build
	cd LULESH_1 && mkdir build
	cd LULESH_1/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_1/build && /usr/bin/time --append --output=../../measurements.csv --format="1;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

2:
	cd LULESH_2 && rm -rf build
	cd LULESH_2 && mkdir build
	cd LULESH_2/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_2/build && /usr/bin/time --append --output=../../measurements.csv --format="2;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

3:
	cd LULESH_3 && rm -rf build
	cd LULESH_3 && mkdir build
	cd LULESH_3/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_3/build && /usr/bin/time --append --output=../../measurements.csv --format="3;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

4:
	cd LULESH_4 && rm -rf build
	cd LULESH_4 && mkdir build
	cd LULESH_4/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_4/build && /usr/bin/time --append --output=../../measurements.csv --format="4;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

5:
	cd LULESH_5 && rm -rf build
	cd LULESH_5 && mkdir build
	cd LULESH_5/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_5/build && /usr/bin/time --append --output=../../measurements.csv --format="5;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

6:
	cd LULESH_6 && rm -rf build
	cd LULESH_6 && mkdir build
	cd LULESH_6/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_6/build && /usr/bin/time --append --output=../../measurements.csv --format="6;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

7:
	cd LULESH_7 && rm -rf build
	cd LULESH_7 && mkdir build
	cd LULESH_7/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_7/build && /usr/bin/time --append --output=../../measurements.csv --format="7;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

8:
	cd LULESH_8 && rm -rf build
	cd LULESH_8 && mkdir build
	cd LULESH_8/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_8/build && /usr/bin/time --append --output=../../measurements.csv --format="8;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

9:
	cd LULESH_9 && rm -rf build
	cd LULESH_9 && mkdir build
	cd LULESH_9/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_9/build && /usr/bin/time --append --output=../../measurements.csv --format="9;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

10:
	cd LULESH_10 && rm -rf build
	cd LULESH_10 && mkdir build
	cd LULESH_10/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_10/build && /usr/bin/time --append --output=../../measurements.csv --format="10;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

14:
	cd LULESH_14 && rm -rf build
	cd LULESH_14 && mkdir build
	cd LULESH_14/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_14/build && /usr/bin/time --append --output=../../measurements.csv --format="14;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

15:
	cd LULESH_15 && rm -rf build
	cd LULESH_15 && mkdir build
	cd LULESH_15/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_15/build && /usr/bin/time --append --output=../../measurements.csv --format="15;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

16:
	cd LULESH_16 && rm -rf build
	cd LULESH_16 && mkdir build
	cd LULESH_16/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_16/build && /usr/bin/time --append --output=../../measurements.csv --format="16;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

17:
	cd LULESH_17 && rm -rf build
	cd LULESH_17 && mkdir build
	cd LULESH_17/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_17/build && /usr/bin/time --append --output=../../measurements.csv --format="17;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

18:
	cd LULESH_18 && rm -rf build
	cd LULESH_18 && mkdir build
	cd LULESH_18/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_18/build && /usr/bin/time --append --output=../../measurements.csv --format="18;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

19:
	cd LULESH_19 && rm -rf build
	cd LULESH_19 && mkdir build
	cd LULESH_19/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_19/build && /usr/bin/time --append --output=../../measurements.csv --format="19;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

20:
	cd LULESH_20 && rm -rf build
	cd LULESH_20 && mkdir build
	cd LULESH_20/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_20/build && /usr/bin/time --append --output=../../measurements.csv --format="20;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

21:
	cd LULESH_21 && rm -rf build
	cd LULESH_21 && mkdir build
	cd LULESH_21/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_21/build && /usr/bin/time --append --output=../../measurements.csv --format="21;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

22:
	cd LULESH_22 && rm -rf build
	cd LULESH_22 && mkdir build
	cd LULESH_22/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_22/build && /usr/bin/time --append --output=../../measurements.csv --format="22;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

23:
	cd LULESH_23 && rm -rf build
	cd LULESH_23 && mkdir build
	cd LULESH_23/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_23/build && /usr/bin/time --append --output=../../measurements.csv --format="23;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

24:
	cd LULESH_24 && rm -rf build
	cd LULESH_24 && mkdir build
	cd LULESH_24/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_24/build && /usr/bin/time --append --output=../../measurements.csv --format="24;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

25:
	cd LULESH_25 && rm -rf build
	cd LULESH_25 && mkdir build
	cd LULESH_25/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_25/build && /usr/bin/time --append --output=../../measurements.csv --format="25;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

26:
	cd LULESH_26 && rm -rf build
	cd LULESH_26 && mkdir build
	cd LULESH_26/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_26/build && /usr/bin/time --append --output=../../measurements.csv --format="26;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

27:
	cd LULESH_27 && rm -rf build
	cd LULESH_27 && mkdir build
	cd LULESH_27/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_27/build && /usr/bin/time --append --output=../../measurements.csv --format="27;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

28:
	cd LULESH_28 && rm -rf build
	cd LULESH_28 && mkdir build
	cd LULESH_28/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_28/build && /usr/bin/time --append --output=../../measurements.csv --format="28;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

34:
	cd LULESH_34 && rm -rf build
	cd LULESH_34 && mkdir build
	cd LULESH_34/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_34/build && /usr/bin/time --append --output=../../measurements.csv --format="34;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

35:
	cd LULESH_35 && rm -rf build
	cd LULESH_35 && mkdir build
	cd LULESH_35/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_35/build && /usr/bin/time --append --output=../../measurements.csv --format="35;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

40:
	cd LULESH_40 && rm -rf build
	cd LULESH_40 && mkdir build
	cd LULESH_40/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_40/build && /usr/bin/time --append --output=../../measurements.csv --format="40;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

41:
	cd LULESH_41 && rm -rf build
	cd LULESH_41 && mkdir build
	cd LULESH_41/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_41/build && /usr/bin/time --append --output=../../measurements.csv --format="41;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}

43:
	cd LULESH_43 && rm -rf build
	cd LULESH_43 && mkdir build
	cd LULESH_43/build && cmake .. -DWITH_MPI=Off -DWITH_OPENMP=Off && make -j
	cd LULESH_43/build && /usr/bin/time --append --output=../../measurements.csv --format="43;%e;%x;" ./lulesh2.0 -s ${LULESH_PROBLEM_SIZE}
