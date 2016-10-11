all: directories
	cd plugins; make
	cd bin/obj_gui; qmake ../../align_gui/align_gui.pro; make;
	cd bin/obj_console; qmake ../../align_console/align_console.pro; make;
	cp bin/obj_gui/align_gui bin/align_gui
	cp bin/obj_console/align_console bin/align_console

directories: 
	mkdir -p bin
	mkdir -p bin/obj_gui
	mkdir -p bin/obj_console

clean:
	cd plugins; make clean
	rm -r -f bin
