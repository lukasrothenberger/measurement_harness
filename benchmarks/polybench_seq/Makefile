DIRS=$$(find . -maxdepth 1 -type d -not -name .)

all:
	for f in ${DIRS}; do \
		cd $$f && make all && cd .. ;\
	done

cpu:
	for f in ${DIRS}; do \
		cd $$f && (make cpu || true) && cd .. ;\
	done

gpu:
	for f in ${DIRS}; do \
		cd $$f && make gpu && cd .. ;\
	done

clean:
	for f in ${DIRS}; do \
		cd $$f && make clean && cd .. ;\
	done
	
