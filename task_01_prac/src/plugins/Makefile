all: directories
	cd medianfilter; make
	cd autocontrast; make
	cp medianfilter/medianfilter.so bin/medianfilter.so
	cp autocontrast/autocontrast.so bin/autocontrast.so

directories: 
	mkdir -p bin

clean:
	cd medianfilter; make clean
	cd autocontrast; make clean
	rm -r -f bin
