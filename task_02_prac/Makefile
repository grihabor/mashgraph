RUN_COUNT = 100

all: 	
	make build
	rm -f speed_up.txt
	# recursively extract features
	find data -name '*.bmp' -exec python run.py '{}' $(RUN_COUNT) \;
	python average.py speed_up.txt speed up

	
CUR_PATH = $(shell pwd)

build:
	cd template; make all
clean:
	cd template; make clean
test:
	cd template; make test
	
