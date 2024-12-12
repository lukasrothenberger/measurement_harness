DIRS=$$(find . -maxdepth 1 -type d -not -name .)

all:
	for f in ${DIRS}; do \
		cd $$f && make get_code compile execute plot && cd .. ;\
	done

get_code:
	for f in ${DIRS}; do \
		cd $$f && make get_code && cd .. ;\
	done

compile:
	for f in ${DIRS}; do \
		cd $$f && make compile && cd .. ;\
	done

execute:
	for f in ${DIRS}; do \
		cd $$f && make execute && cd .. ;\
	done

plot:
	for f in ${DIRS}; do \
		cd $$f && make plot && cd .. ;\
	done

clean:
	for f in ${DIRS}; do \
		cd $$f && make clean && cd .. ;\
	done
	

veryclean:
	for f in ${DIRS}; do \
		cd $$f && make veryclean && cd .. ;\
	done
	
