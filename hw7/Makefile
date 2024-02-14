calc :  liblibcalc.a
	gcc main.c -o calc -L.  -llibcalc
liblibcalc.a : add.o sub.o mul.o div.o
	ar rc liblibcalc.a add.o sub.o mul.o div.o
add.o : add.c
		gcc add.c -c
sub.o : sub.c
		gcc sub.c -c
mul.o : mul.c
		gcc mul.c -c
div.o : div.c
		gcc div.c -c


run: calc
	./calc
clean: 
	rm *.a
	rm *.o
	rm calc