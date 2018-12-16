#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 1000
#define NUM_RANGE 50

int do_open() {
  int s;
  // Get Address stuff
  struct addrinfo hints, *result;

  // Setting up the hints struct...
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  s = getaddrinfo(NULL, "1412", &hints, &result);
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
	
  // Step 5: Accept a connection
  printf("Waiting for connection...\n");
  int client_fd = accept(sock_fd, NULL, NULL);
  printf("Connection made: client_fd=%d\n", client_fd);

  return client_fd;
}

// Receive message from client
void receive_message(int client_fd, char* msg) {
	
  char buffer[BUFSIZE];
  int len = read(client_fd, buffer, BUFSIZE - 1);
  buffer[len] = '\0';
  printf("CLIENT: %s \n", buffer);

  strncpy(msg, buffer, BUFSIZE);
}

// Send message to client
void send_message(char *msg, int sock_fd) {
  printf("SERVER: %s \n", msg);
  printf("===\n");
  write(sock_fd, msg, strlen(msg));
}

// Tranfer given num to string
char* itoa(int n) {
  char* ret = NULL;
  int numChars = 0;
  // Determine if integer is negative
  int isNegative = 0;
  if (n < 0) {
    n = -n;
    isNegative = 1;
    numChars++;
  }
  // Count how much space we will need for the string
  int temp = n;
  do {
    numChars++;
    temp /= 10;
  } while ( temp );
  //Allocate space for the string
  ret = (char*) malloc(sizeof(char) * (numChars + 1));
  ret[numChars] = 0;
  //Add the negative sign if needed
  if (isNegative) ret[0] = '-';
  // Copy digits to string in reverse order
  int i = numChars - 1;
  do {
    ret[i--] = n%10 + '0';
    n /= 10;
  } while (n);
  return ret;
}

int main() {
  printf("This is the server side!\n\n");	
  int client_socket_id = do_open();

  // A seed to generate random target
  srand(time(NULL));

  // Generate a random target in [1, NUM_RANGE]
  int target_num = rand() % NUM_RANGE + 1;
  printf("Target to guess is: %d\n", target_num);
  char welcome_words[100] = "Welcome! I have choose a number in 1 and ";
  strcat(welcome_words, itoa(NUM_RANGE));

  send_message(welcome_words, client_socket_id);

  // Keep record of the total times of guess of the client	
  int time_of_guess = 0;

  // Allocate memory for response message
  char* resp;
  resp = (char*) malloc (sizeof(char) * BUFSIZE);

  while (1) {
    receive_message(client_socket_id, resp);
    time_of_guess++;
    int value = atoi(resp);
    if (value == 0) {
      send_message("I don't understand the guess", client_socket_id);
    } else {
      if (value < target_num) {
	send_message("too low", client_socket_id);			
      } else if (value > target_num) {
	send_message("too high", client_socket_id);
      } else {
	break;
      }
    }
  }
	
  // Prepare the congratulation words to the client
  char congrat_words[100] = "you guessed my secrete number it only takes you ";
  strcat(congrat_words, itoa(time_of_guess));
  strcat(congrat_words, " guesses, plus something I didn't understand!");
  send_message(congrat_words, client_socket_id);
  				
  // Close the socket
  close(client_socket_id);
  free(resp);

  return 0; 
}





