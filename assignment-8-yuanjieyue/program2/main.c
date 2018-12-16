#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 50
#define SIZE 1000
#define FLAG_P "-p"
#define FLAG_R "-r"
#define FLAG_A "-a"

// The struct to hold the data reading from the file
typedef struct player_data {
  char* player_name;
  int year;
  float average_points;
  float average_rebounds;
  float average_assists;
} Player_Data;


// Comparator on average points on the struct
int comparePoints(const void *pa, const void *pb) {
  Player_Data *pda = *(Player_Data**)pa;
  Player_Data *pdb = *(Player_Data**)pb;  
  return (((pda->average_points) > (pdb->average_points)) ? -1 : 1);
}

// Comparator on average rebounds on the struct
int compareRebounds(const void *ra, const void *rb) {
  Player_Data *pda = *(Player_Data**)ra;
  Player_Data *pdb = *(Player_Data**)rb;  
  return (((pda->average_rebounds) > (pdb->average_rebounds)) ? -1 : 1);
  
}

// Comparator on average assists on the struct
int compareAssists(const void *aa, const void *ab) {
  Player_Data *pda = *(Player_Data**)aa;
  Player_Data *pdb = *(Player_Data**)ab;  
  return (((pda->average_assists) > (pdb->average_assists)) ? -1 : 1);
}

void copy_from_file_to_struct(char* str, Player_Data* cur_player_data);

// Main funtion to run the program
int main(int argc, char* argv[]) {
  
  // Check if there are two command line arguments supplied
  if (argc == 3) {
    // If wrong flag supplied, print wrong message
    if (strcmp(argv[1], FLAG_P) != 0 && strcmp(argv[1], FLAG_R) != 0 
       && strcmp(argv[1], FLAG_A) != 0) {
       printf("Wrong flag, three flags supported: '-p', '-r', '-a'.\n");
    } else {

      // Open the input file that is supplied as command line argument
      FILE* fp = fopen(argv[2], "r");
      Player_Data* player_datas[N];

      char tmp[SIZE];
      int num = 0;
      // Get datas from the input file
      while (fgets(tmp, SIZE, fp) != NULL) {
        // Malloc memory for each struct play data in the struct array
        Player_Data* cur_player_data = (Player_Data*)malloc(sizeof(Player_Data));
        // Copy datas from file to the struct
        copy_from_file_to_struct(tmp, cur_player_data);
        // Assign the struct into the array
        player_datas[num] = cur_player_data;
        num++;
      }
      // Now num is total num of structs in the array
      
      // Pick the sort methods base on the flag passed as command line argument
      if (strcmp(argv[1], FLAG_P) == 0) {
        qsort(player_datas, num, sizeof(player_datas[0]), comparePoints);
      } else if (strcmp(argv[1], FLAG_R) == 0) {
        qsort(player_datas, num, sizeof(player_datas[0]), compareRebounds);
      } else {
        qsort(player_datas, num, sizeof(player_datas[0]), compareAssists);
      }

      // Print out the result to screen
      int i;
      for (i = 0; i < num; i++) {
        printf("%s,%d,%.1f,%.1f,%.1f\n", player_datas[i]->player_name, 
        player_datas[i]->year, player_datas[i]->average_points, 
        player_datas[i]->average_rebounds, player_datas[i]->average_assists);
      }
      fclose(fp);
    }
  }
  else if (argc < 3) {
    printf("Two arguments expected!\n");
  }
  else {
    printf("Too many arguments supplied!\n");
  }
  
  return 0;
}

// Function that copy data from input file into struct
void copy_from_file_to_struct(char* str, Player_Data* cur_player_data) {
  // Take ',' as the spliter.
  char delims[] = ",";
  char *result = NULL;
  result = strtok(str, delims);
  int count = 0;
  // Copy each data based on their position in input string
  while (result != NULL) { 
    count++;
    switch (count) {
      case 1: 
        //printf("Copy player name\n");
        cur_player_data->player_name = (char*)malloc(sizeof(char) * strlen(result));
        strcpy(cur_player_data->player_name, result);
        break;
      case 2:
        //printf("Copy year\n");
        cur_player_data->year = atoi(result);
        break;
      case 3:
        //printf("Copy average porints\n");
        cur_player_data->average_points = atof(result);
        break;
      case 4:
        //printf("Copy average rebounds\n");
        cur_player_data->average_rebounds = atof(result);
        break;
      case 5:
        //printf("Copy average assists\n");
        cur_player_data->average_assists = atof(result);
        break;
      default:
        break;
    }
    result = strtok(NULL, delims);    
  }
}









