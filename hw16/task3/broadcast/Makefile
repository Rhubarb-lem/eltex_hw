server :  server.c
	gcc server.c -o server 
client :  client.c
	gcc client.c -o client 


all : server client
runserver: server
	sudo ./server
	
runclient: client
	./client
clean: 
	rm client
	rm server

	