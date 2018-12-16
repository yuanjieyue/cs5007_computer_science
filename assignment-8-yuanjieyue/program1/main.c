#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define N 10

int read_all_nums_from_dir(char* argu, FILE* file);
char* string_concat(char* a, char* b);
void to_char_array(char** a, FILE* file);
void to_int_array(int* b, char** a, int n);
void merge_sort(int arr[], int low, int high, int len);
void msort(int src[], int des[], int low, int high, int max_size);
void merge(int src[], int des[], int low, int high, int mid);

// Main function to run the program, 
// argc is the number of command line arguments.
// argv[] is the command line arguments.
int main(int argc, char *argv[]) {

  // If there is one command line argument passed.
  if (argc == 2) {
    FILE *res = fopen("result.txt", "a+");
    // Read all contents from the directory passed as command line argument.
    int count = read_all_nums_from_dir(argv[1], res);
    //printf("the total nums of integers in dir is %d!\n", count);
    fclose(res);    
    
    // Read all contents to a char* array line by line.
    FILE *p = fopen("result.txt", "r");
    char* char_array[count];
    to_char_array(char_array, p);   
    fclose(p);

    //Transfer the char* nums into int nums.
    int* int_array = (int*)malloc(count * sizeof(int));
    to_int_array(int_array, char_array, count);
    
    //Merge sort the int nums.
    //printf("%s", "The Result is : \n");
    merge_sort (int_array, 0, count, count);
    int j;
    for (j = 0; j < count; j++) {
      printf("the %dth elements in result file is:  %d\n", j, int_array[j]);
    }    
    
    // Write the result into the result.txt file.  
    FILE *ppp = fopen("result.txt", "w");
    j = 0;
    while (j < count) {
      fprintf(ppp, "%d\n", int_array[j]);
      j++;
    }
    fclose(ppp); 
  }
  else if (argc > 2) {
    printf("Too many arguments supplied.\n");
  }
  else {
    printf("One argument expected.\n");
  } 
  return 0;
}

char* string_concat(char* a, char* b) {
  // Get the length of string a and b.
  int len1 = strlen(a);
  int len2 = strlen(b);
  // Malloc memory for result string.
  char* file_name = (char*)malloc((len1 + len2 + 3) * sizeof(char));
  strcpy(file_name, "./");
  strcat(file_name, a);
  strcat(file_name, "/");
  strcat(file_name, b);  
  return file_name;
}

// Transfer the givn char* array into an integer array.
void to_int_array(int* b, char** a, int n) {
  int i;
  for (i = 0; i < n; i++) {
    *(b + i) = atoi(*(a + i));
  }
}


int read_all_nums_from_dir(char* argu, FILE* file) {
  DIR *dir;
  struct dirent *ptr;
  // Open the directory.
  dir = opendir(argu);
  // Intialize a counter to hold the total nums of number in the directory.
  int count = 0;
  while ((ptr = readdir(dir)) != NULL) {
    if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
      continue;
    } 
    // Get every file name in this directory.   
    char* file_name = string_concat(argu, ptr->d_name); 
    //printf("%s\n", file_name);

    // Open the file with the file name. 
    FILE *fp;
    fp = fopen(file_name, "r");
     
    if (fp == NULL) {
      printf("there is an error in opening the file");
      exit(1);
    }
    // Initialize a holder for the string at each line.
    char str[N + 1];
    while (fgets(str, N, fp) != NULL) {  
      count++;
      //printf("%d ", count);
      // Write the string of each line into the target file.
      fputs(str, file);
    }
    fclose(fp);    
  }
  closedir(dir);

  return count;
}
    
// Read the contents into the target char array
void to_char_array(char** a, FILE* file) {
  char ch[N + 1];
  int i = 0;

  while (fgets(ch, N, file) != NULL) {
    //printf("%d ", i);
    char* tmp = (char*)malloc((strlen(ch) + 1) * sizeof(char));
    strcpy(tmp, ch);
    a[i++] = tmp;
  }
}

// Merge sort the nums in the given array arr[].
void merge_sort(int arr[], int low, int high, int len) {
  msort(arr, arr, low, high, len);
} 

void msort(int src[], int des[], int low, int high, int max_size) {
  int mid = (low + high) / 2;
  if (low == high) {
    des[low] = src[low];
  }
  else {
    int * des_space = (int*)malloc(sizeof(int) * max_size);
    if (NULL != des_space) {
      msort(src, des_space, low, mid, max_size);
      msort(src, des_space, mid + 1, high, max_size);
      merge(des_space, des, low, high, mid);
    }
    free(des_space); 
  }
}

void merge(int src[], int des[], int low, int high, int mid) {
  int i = low;
  int k = low;
  int j = mid + 1;

  while (( i <= mid ) && (j <= high)) {
    if (src[i] < src[j]) {
      des[k++] = src[i++];
    }
    else {
      des[k++] = src[j++];
    }
  }
  while (i <= mid) {
    des[k++] = src[i++];
  }
  while (j <= high) {  
    des[k++] = src[j++];
  }
  
}


