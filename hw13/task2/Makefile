server :  server.c
	gcc server.c -o server -lrt -lpthread
client :  client.c
	gcc client.c -o client -lrt -lpthread -lncurses


all : server client
runserver: server
	./server
	
runclient: client
	./client
clean: 
	rm client
	rm server

	