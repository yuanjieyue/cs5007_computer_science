package edu.neu.cs5007;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.*;
import java.util.InputMismatchException;
import java.util.Random;
import java.util.Scanner;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;

public class Client {
  private static final int CLIENT_MARK = 10;
  private static final int SERVER_MARK = 0;

  // The url is against the endpoint in the server that process with the game
  private static String URL = "http://localhost:8080/run";

  private static String JSON_CONTENT_TYPE_NAME_STRING = "Content-type";
  private static String JSON_CONTENT_TYPE_VALUE_STRING = "application/json";

  static final Random RAND = new Random();
  static final Scanner IN = new Scanner(System.in);

  // Main method run the client side
  public static void main(String[] args) {

    Client client = new Client();
    CloseableHttpClient httpClient = HttpClientBuilder.create().build();
    Gson gson = new GsonBuilder().create();

    Game clientGame = new Game();
    int start = RAND.nextInt(2);

    // if start is 0, server goes first
    // if start is 1 client goes first
    if (start == 1) {
      System.out.println(clientGame);
      System.out.println("\nClient goes first, please choose a slot: \n");
      int slot = clientGame.chooseSlotFromInput(IN);
      clientGame.play(slot);
      System.out.println(clientGame);
    }

    System.out.println("Server's turn: \n");
    Game receivedGame = client.send(httpClient, URL, gson, clientGame);

    int gameGoingOn = clientGame.checkWinner();
    // If game is going on, the client keep receive and send messages
    while (gameGoingOn == 0) {
      System.out.println(receivedGame);
      gameGoingOn = receivedGame.checkWinner();

      if (gameGoingOn == -1) {
        System.out.println("Game Over: Winner is the server!");
        break;
      } else if (gameGoingOn == 1) {
        System.out.println("Something is wrong, the client should never win!");
        break;
      }

      if (receivedGame.isGameOver()) {
        System.out.println("Game Over: It's a draw!");
        break;
      }
      System.out.println("Now client's turn, please choose a slot: \n");
      // Get the slot to play from user input
      int nextSlot = receivedGame.chooseSlotFromInput(IN);

      receivedGame.play(nextSlot);
      System.out.println(receivedGame);
      // After the client play at the chosen slot
      // if the board if not full, send the new game to the server
      if (!receivedGame.isBoardFull()) {
        System.out.println("Now server's turn: \n");
        receivedGame = client.send(httpClient, URL, gson, receivedGame);
      }
      // if the board is full,check if server or client win
      // if no one wins, it is a draw
      else {
        int winner = receivedGame.checkWinner();
        if(winner != -1 && winner != 1){
          System.out.println("it is a draw, game over");
          break;
        }
      }
    }
  }

  // Given the entity of the http post
  // Return the content from the http entity
  private String printHTTPEntity(HttpEntity entity) {
    String completePayload = "";
    try {
      byte[] buffer = new byte[1024];
      if (entity != null) {
        InputStream inputStream = entity.getContent();
        try {
          int bytesRead = 0;
          BufferedInputStream bis = new BufferedInputStream(inputStream);
          while ((bytesRead = bis.read(buffer)) != -1) {
            String chunk = new String(buffer, 0, bytesRead);
            System.out.println(chunk);
            completePayload += chunk;
          }
        } catch (Exception e) {
          e.printStackTrace();
        } finally {
          try {
            inputStream.close();
          } catch (Exception ignore) {
          }
        }
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
    return completePayload;
  }

  // Given the http client, the server url, gson and the game to be sent
  // Return the message from server to client.
  private Game send(CloseableHttpClient httpClient, String url,
		    Gson gson, Game game) {

    String jsonString = gson.toJson(game);

    HttpPost postRequest = new HttpPost(url);
    postRequest.setHeader(JSON_CONTENT_TYPE_NAME_STRING,
			  JSON_CONTENT_TYPE_VALUE_STRING);
    // Set the payload of the request
    try {
      postRequest.setEntity(new StringEntity(jsonString));
    } catch (UnsupportedEncodingException e) {
      e.printStackTrace();
    }
    // Execute the request
    HttpResponse httpResponse = null;
    try {
      httpResponse = httpClient.execute(postRequest);
    } catch (IOException e) {
      e.printStackTrace();
    }
    HttpEntity postEntity = httpResponse.getEntity();
    String resp = printHTTPEntity(postEntity);

    return gson.fromJson(resp, Game.class);
  }
}

// The class act as the message of the client
// Could be transfered to json object when sent to the server
class Game {
  private static final int SLOTS_OF_BOARD = 9;

  private static final int CLIENT_MARK = 10;
  private static final int SERVER_MARK = 0;

  private int[] board;

  public Game() {
    this.board = new int[SLOTS_OF_BOARD];
    populateEmptyBoard(this.board);
  }

  // Given the board to be populate
  // Return the board being populated with number
  public void populateEmptyBoard(int[] board) {
    for (int i = 0; i < board.length; i++) {
      board[i] = i + 1;
    }
  }
  // Getter for the board
  public int[] getBoard() {
    return board;
  }
  // Setter for the board
  public void setBoard(int[] board) {
    this.board = board;
  }
  // Given the slot that the client choose
  // set the number on the slot to be the client mark
  public void play(int slot) {
    this.board[slot - 1] = CLIENT_MARK;
  }
  // Given the input scanner
  // Get back the a valid input from the client
  public int chooseSlotFromInput(Scanner in) {
    String slot;
    slot = in.nextLine();
    while (!isSlotValid(slot)) {
      System.out.println("Invalid input; re-enter slot number:");
      slot = in.nextLine();
    }
    return Integer.parseInt(slot);
  }
  // Given a client's string input
  // Check if slot that the string represents is valid or not
  public boolean isSlotValid(String input) {
    if (input.length() > 1) {
      return false;
    }
    if(!Character.isDigit(input.charAt(0))){
      return false;
    }
    int slot = Integer.parseInt(input);
    if (!(slot >= 1 && slot <= 9)) {
      return false;
    }
    if (board[slot - 1] != slot) {
      return false;
    }
    this.play(slot);
    if (checkWinner() == 1) {
      this.board[slot - 1] = slot;
      System.out.println("client will win if you play at this slot");
      return false;
    } else {
      this.board[slot - 1] = slot;
      return true;
    }
  }

  // Check if game is over
  public boolean isGameOver() {
    return isBoardFull() || checkWinner() != 0;
  }

  // Check if the board is full or not
  public boolean isBoardFull() {
    for (int i = 0; i < board.length; i++) {
      if (board[i] == i + 1) {
        System.out.println("Board is not full!\n");
        return false;
      }
    }
    return true;
  }

  // Check if there is a Winner
  // return 0 for game still going on
  //        1 for client is the winner
  //        -1 for the server is the winner
  public int checkWinner() {
    for (int a = 0; a < 8; a++) {
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

  // Print out the board into a human readable board on terminal
  @Override
  public String toString() {
    String[] str = new String[SLOTS_OF_BOARD];
    for (int i = 0; i < SLOTS_OF_BOARD; i++) {
      if (board[i] == i + 1) {
        str[i] = Integer.toString(i + 1);
      }
      if (board[i] == SERVER_MARK) {
        str[i] = "O";
      }

      if (board[i] == CLIENT_MARK) {
        str[i] = "X";
      }
    }
    return "The play board is: \n\n" +
          "   Client[X] -- Server[O]\n" +
          "\t/---|---|---\\\n" +
          "\t| " + str[0] + " | " + str[1] + " | " + str[2] + " |\n" +
          "\t|-----------|\n" +
          "\t| " + str[3] + " | " + str[4] + " | " + str[5] + " |\n" +
          "\t|-----------|\n" +
          "\t| " + str[6] + " | " + str[7] + " | " + str[8] + " |\n" +
          "\t/---|---|---\\\n";
  }
}
