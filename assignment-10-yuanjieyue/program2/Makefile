all: client server

client: guess_client.c
	gcc -o client guess_client.c

server: guess_server.c
	gcc -o server guess_server.c

run_client:
	./client

run_server:
	./server

.PHONY: clean

clean: 
	rm -f ./client ./server
