server :  server.c
	gcc server.c -o server -lrt
client :  client.c
	gcc client.c -o client -lrt


all : server client
runserver: server
	./server
	
runclient: client
	./client
clean: 
	rm client
	rm server

	