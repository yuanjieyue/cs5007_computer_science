#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 1000
#define INPUTSIZE 100
#define NUM_RANGE 50

int do_connect(char *host, char *port) {
  int s;
  // Create a socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	
  struct addrinfo hints, *result;

  // Initialize the addrinfo variable
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  // Get address info and set the info to the variable result
  s = getaddrinfo(host, port, &hints, &result);
  // If I can't get the address, write an error
  if (s != 0) {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }

  // Try to connect: if I can't, write an error
  if (connect (sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
    perror("connect()");
    exit(1);
  }
  else {
    printf("Connection is good!\n");
  }

  return sock_fd;
}

// Read the response from the server
void read_response(int sock_fd, char* msg) {
  char resp[BUFSIZE];
  int len;
  len = read(sock_fd, resp, BUFSIZE - 1);
  resp[len] = '\0';
  printf("SERVER: %s \n", resp);
  printf("===\n");
  strncpy(msg, resp, BUFSIZE);
}

// Send message to the server
void send_message(char *msg, int sock_fd) {
  printf("CLIENT: %s \n", msg);
  printf("===\n");
  write(sock_fd, msg, strlen(msg));
}


int main() {
  printf("Here is the client side!\n\n");
  int sock_fd = do_connect("localhost", "1412");

  // Allocat memory to hold response from the server
  char* resp;
  resp = (char*)malloc(sizeof(char) * BUFSIZE);

  read_response(sock_fd, resp);	
  while (1) {		
    printf("Input the num you guess: ");
    char msg[INPUTSIZE];
    scanf("%s", msg);

    send_message(msg, sock_fd);
    read_response(sock_fd, resp);
    // If the response is not the three indicate words, break the loop
    char ind_1[100] = "too low";
    char ind_2[100] = "too high";
    char ind_3[100] = "I don't understand the guess";
    if (strcmp(resp,ind_1)!=0 && strcmp(resp,ind_2)!=0 && strcmp(resp,ind_3)!=0) {
      break;
    };				
  }
	
  // Close the socket
  close(sock_fd);

  free(resp);

  return 0; 
}


















