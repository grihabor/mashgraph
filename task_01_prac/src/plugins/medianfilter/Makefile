all: medianfilter.so

medianfilter.so: medianfilter.o
	g++ -shared -o medianfilter.so medianfilter.o

medianfilter.o: medianfilter.h medianfilter.cpp ../../shared/headers/pluginfilter.h
	g++ -Wall -Werror -std=c++11 -fPIC -c -o medianfilter.o medianfilter.cpp

clean: 
	rm -rf medianfilter.o medianfilter.so
