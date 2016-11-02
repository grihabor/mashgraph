all: 	
	make build
	
	# recursively extract features
	find data -name '*.bmp' -exec template/build/bin/task2 '{}' \;
	

	
CUR_PATH = $(shell pwd)

build:
	cd template; make all
clean:
	cd template; make clean
