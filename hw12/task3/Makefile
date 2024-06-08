shop :  libsplit_args.a libsplit_commands.a
	gcc interpreter.c -o interpreter -L.  -lsplit_args -lsplit_commands
split_commands.o : split_commands.c
		gcc split_commands.c -c 
libsplit_commands.a : split_commands.o
	ar rc libsplit_commands.a split_commands.o  
split_args.o : split_args.c
		gcc split_args.c -c 
libsplit_args.a : split_args.o
	ar rc libsplit_args.a split_args.o  	

run: interpreter
	./interpreter
clean: 
	rm *.a
	rm *.o
	rm interpreter