DIRS=$$(find . -maxdepth 1 -type d -not -name .)

all:
	for f in ${DIRS}; do \
		cd $$f && make get_code compile execute plot || true && cd .. ;\
	done

get_code:
	for f in ${DIRS}; do \
		cd $$f && make get_code || true  && cd .. ;\
	done

compile:
	for f in ${DIRS}; do \
		cd $$f && make compile || true && cd .. ;\
	done

execute:
	for f in ${DIRS}; do \
		cd $$f && make execute || true && cd .. ;\
	done

plot:
	for f in ${DIRS}; do \
		cd $$f && make plot || true && cd .. ;\
	done

clean:
	for f in ${DIRS}; do \
		cd $$f && make clean || true && cd .. ;\
	done
	

veryclean:
	for f in ${DIRS}; do \
		cd $$f && make veryclean || true && cd .. ;\
	done
	
