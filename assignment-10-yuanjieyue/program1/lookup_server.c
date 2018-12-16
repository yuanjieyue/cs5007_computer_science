#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1000

int do_open() {
  int s;
  // Get Address stuff
  struct addrinfo hints, *result;

  // Setting up the hints struct...
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  s = getaddrinfo(NULL, "1413", &hints, &result);
  if (s != 0) {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }

  // Create the socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Set socket opt on so the port num could be use for multiple times.
  int on = 1;
  if ((setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
    perror("setsockopt failed\n");
    exit(EXIT_FAILURE);
  } 

  // Bind the socket
  if (bind (sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind()");
    exit(1);
  }
	
  // Listen
  if (listen (sock_fd, 10) != 0) {
    perror("listen()");
    exit(1);
  }

  struct sockaddr_in *result_addr = (struct sockaddr_in *) result->ai_addr;
  printf("Listening on file descriptor %d, port %d\n", sock_fd, ntohs(result_addr->sin_port));
	
  // Accept a connection
  printf("Waiting for connection...\n");
  int client_fd = accept(sock_fd, NULL, NULL);
  printf("Connection made: client_fd=%d\n", client_fd);

  // Return a client identifier
  return client_fd;
}

// Receive message from client
void receive_message(int client_fd, char* msg) {
  char buffer[BUFSIZE];
  int len = read(client_fd, buffer, BUFSIZE - 1);
  buffer[len] = '\0';
  printf("SERVER RECEIVED: %s \n", buffer);

  strncpy(msg, buffer, BUFSIZE);
}

// Send message to client
void send_message(char *msg, int sock_fd) {
  printf("SERVER SENDING: %s \n", msg);
  printf("===\n");
  write(sock_fd, msg, strlen(msg));
}


int main() {
  printf("This is the server side!\n\n");	

  int client_socket_id = do_open();

  // Read a message from the client
  char* msg;
  msg = (char*)malloc(sizeof(char) * BUFSIZE);

  receive_message(client_socket_id, msg);
  printf("\nthe message server received is: %s\n", msg);
	
  // Look up the dns
  struct addrinfo hints, *infoptr;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
	
  int result = getaddrinfo(msg, NULL, &hints, &infoptr);
  if (result) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
    exit(1);
  }	
  struct addrinfo *p;
  char host[256];
	
  for (p = infoptr; p != NULL; p = p->ai_next) {
    getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
    send_message(host, client_socket_id);
  }
  freeaddrinfo(infoptr);
  				
  // Close the socket
  close(client_socket_id);
	
  free(msg);
  return 0; 
}





