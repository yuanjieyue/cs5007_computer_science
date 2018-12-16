#include <stdio.h>
#include <ulfius.h>

// port number
#define PORT 8080

#define CLIENT_MARK 10
#define SERVER_MARK 0
#define NUM_OF_SLOT 9

int shutdown_now = 0;

int declare_winner(int* board);
int one_move_to_win(int* board, int player_mark);
int next_corner_slot(int* board);
int next_edge_slot(int* board);
void print_array(int* board);

// process information from user, given a http request, response and user data
// send the processed information back to client
int callback_process_game(const struct _u_request * request,
  struct _u_response * response, void * user_data) {
  json_auto_t * json_game = ulfius_get_json_body_request(request, NULL);
  if(!json_game) {
    printf("json_game is null, exiting\n");
  }
  json_auto_t * board;

  // get the JSON object
  board = json_object_get(json_game, "board");
  if (!json_is_array(board)) {
    printf("board value is NOT an array\n");
  }
  // Copy the json array into a normal c array.
  int play_board[NUM_OF_SLOT];
  int i;
  for (i = 0; i < NUM_OF_SLOT; i++) {
    play_board[i] = json_integer_value(json_array_get(board, i));
  }

  printf("\n<------------------Round Start------------------------->\n");

  int slot = 0;
  int server_win_move = one_move_to_win(play_board, SERVER_MARK);
  int client_win_move = one_move_to_win(play_board, CLIENT_MARK);
  if(server_win_move != -1){
    slot = server_win_move;
    printf("\nThe server is going to win by playing at %d.\n", slot + 1);
  }
  else if (client_win_move != -1) {
    slot = client_win_move;
    printf("\nThe client is goint to win, could be blocked at %d.\n", slot + 1);
  }
  else if (play_board[4] == 5){
    slot = 4;
    printf("\nThe center is avaiable, slot at %d.\n", slot + 1);
  }
  else if (next_corner_slot(play_board) != -1) {
    slot = next_corner_slot(play_board);
    printf("\nA corner is avaiable, slot at %d.\n", slot + 1);
  }
  else {
    slot = next_edge_slot(play_board);
    printf("\nOnly edge slot left, slot at %d.\n", slot + 1);
  }
  printf("\nWe play at %d.\n", slot + 1);
  json_array_set(board, slot, json_integer(SERVER_MARK));

  printf("\n<-----------------Round Finish------------------------->\n");

  ulfius_set_json_body_response(response, 200, json_game);
  return U_CALLBACK_CONTINUE;
}

// Search for the next avaiable corner slot to play
int next_corner_slot(int* board) {
  int i;
  for (i = 0; i < NUM_OF_SLOT; i++) {
    if (board[i] == i + 1 && i % 2 == 0) {
      return i;
    }
  }
  return -1;
}
// Search for the next edge slot to play
int next_edge_slot(int* board) {
  int i;
  for (i = 0; i < NUM_OF_SLOT; i++) {
    if (board[i] == i + 1 && i % 2 == 1) {
      return i;
    }
  }
  return -1;
}

// Check if the player with the player mark is one move to win,
// if so, return the slot, play at this slot, if not return -1
int one_move_to_win(int* board, int player_mark){
  int* copy = board;
  for(int i = 0; i < NUM_OF_SLOT; i++){
    if(copy[i] == i + 1){
      // Set next avaiable slot to the player mark
      copy[i] = player_mark;
      if(player_mark == CLIENT_MARK && declare_winner(copy) == 1
        || player_mark == SERVER_MARK && declare_winner(copy) == -1){
        copy[i] = i + 1;
        return i;
      }
      // Set the slot back to its initial value
      copy[i] = i + 1;
    }
  }
  return -1;
}
// Print out a board
void print_array(int* board) {
  int i;
  for (i = 0; i < NUM_OF_SLOT; i++) {
    printf("%d, ", board[i]);
  }
  printf("\n");
}

// Check the winner of the game
// return 0 means game is still runnig
//        1 means client is winner
//       -1 means sever is winner
int declare_winner(int* board) {
  int a;
  // Sum up the 8 straight line on the board, and check if any is completed
  for (a = 0; a < 8; a++) {
    int line = 0;
    switch (a) {
      case 0:
        line = board[0] + board[1] + board[2];
        break;
      case 1:
        line = board[3] + board[4] + board[5];
        break;
      case 2:
        line = board[6] + board[7] + board[8];
        break;
      case 3:
        line = board[0] + board[3] + board[6];
        break;
      case 4:
        line = board[1] + board[4] + board[7];
        break;
      case 5:
        line = board[2] + board[5] + board[8];
        break;
      case 6:
        line = board[0] + board[4] + board[8];
        break;
      case 7:
        line = board[2] + board[4] + board[6];
        break;
    }
    if (line == 3 * CLIENT_MARK) {
      return 1;
    } else if (line == 3 * SERVER_MARK) {
      return -1;
    }
  }
  return 0;
}

/**
 * main function
 */
int main(void) {
  struct _u_instance instance;

  // Initialize instance with the port number
  if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return(1);
  }

  // Endpoint process with the game
  printf("Endpoint created: /run.\n");
  ulfius_add_endpoint_by_val(&instance, "POST", "/run",
    NULL, 0, &callback_process_game, NULL);
  
  // Start the framework
  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    // Wait for the user to press <enter> on the console to quit the application
    printf("Enter any to stop the server\n");

    getchar();
  } else {
    fprintf(stderr, "Error starting framework\n");
  }
  printf("End framework\n");
  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);
  return 0;
}
