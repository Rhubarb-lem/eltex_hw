calc :  liblibcalc.so
	gcc main.c -o calc -L.  -llibcalc
liblibcalc.so : add.o sub.o mul.o div.o
	gcc -shared  add.o sub.o mul.o div.o -o liblibcalc.so
add.o : add.c
		gcc add.c -fPIC -c
sub.o : sub.c
		gcc sub.c -fPIC -c
mul.o : mul.c
		gcc mul.c -fPIC -c
div.o : div.c
		gcc div.c -fPIC -c


run: calc
	LD_LIBRARY_PATH=$$PWD:$$LD_LIBRARY_PATH ./calc
clean: 
	rm *.o
	rm *.so
	rm calc