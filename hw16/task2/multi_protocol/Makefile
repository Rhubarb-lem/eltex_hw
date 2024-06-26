server :  server.c
	gcc server.c -o server 
client_tcp :  client_tcp.c
	gcc client_tcp.c -o client_tcp
client_udp : client_udp.c 
	gcc client_udp.c -o client_udp


all : server client
runserver: server
	./server
	
runclientudp: client_udp
	./client_udp
runclienttcp: client_tcp
	./client_tcp
clean: 
	rm client_tcp
	rm client_udp
	rm server

	