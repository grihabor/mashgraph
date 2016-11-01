all: autocontrast.so

autocontrast.so: autocontrast.o
	g++ -shared -o autocontrast.so autocontrast.o

autocontrast.o: autocontrast.h autocontrast.cpp ../../shared/headers/pluginfilter.h
	g++ -Wall -Werror -std=c++11 -fPIC -c -o autocontrast.o autocontrast.cpp

clean: 
	rm -rf autocontrast.o autocontrast.so
