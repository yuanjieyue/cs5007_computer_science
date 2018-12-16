#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 1000

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
  // Return the socket identifier
  return sock_fd;
}

// Read response from server
void read_response(int sock_fd) {
  char resp[BUFSIZE];
  int len;
  do {
    len = read(sock_fd, resp, BUFSIZE - 1);
    resp[len] = '\0';
    printf("RECEIVED: %s \n", resp);
    printf("===\n");
  } while (len);
}

// Send Message to Server
void send_message(char *msg, int sock_fd) {
  printf("SENDING: %s \n", msg);
  printf("===\n");
  write(sock_fd, msg, strlen(msg));
}


int main() {
  printf("Here is the client side!\n\n");
  int sock_fd = do_connect("localhost", "1413");

  // Read in the clients input from stdin
  printf("Please enter a valid DNS: in the format of like this: \nwww.xxx.com\n: ");
  char msg[BUFSIZE];
  scanf("%s", msg);

  send_message(msg, sock_fd);
  read_response(sock_fd);

  // Close the socket
  close(sock_fd);

  return 0; 
}


















