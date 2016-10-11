all: 
	cd src; make
	rm -r -f bin
	cp src/bin bin -r
	cp src/plugins/bin bin/plugins -r

clean:
	cd src; make clean
	rm -r -f bin

DIR_PATH = $(shell pwd)

run:
	docker run -i -t -v $(DIR_PATH)/bin:/align align bash

build:
	docker build -t align .

build-no-cache:
	docker build --no-cache -t align .
