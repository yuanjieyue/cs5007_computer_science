all: client server

client: lookup_client.c
	gcc -o client lookup_client.c

server: lookup_server.c
	gcc -o server lookup_server.c

run_client:
	./client

run_server:
	./server

.PHONY: clean

clean: 
	rm -f ./client ./server


