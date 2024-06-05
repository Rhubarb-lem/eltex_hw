server :  server.c
	gcc server.c -o server
client :  client.c
	gcc client.c -o client


runserver: server
	./server
	
runclient: client
	./client
clean: 
	rm client
	rm server
	rm taskfifo
	