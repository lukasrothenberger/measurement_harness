cwd := $(shell pwd)

all: cpu

cpu:
	DP_PROJECT_ROOT_DIR=${cwd} ./build_and_measure.sh

gpu:

clean:
	./clean.sh
	rm -rf original_code
