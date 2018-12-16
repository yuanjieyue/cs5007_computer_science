#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>

// Read the file 'input_file.txt', and find the shortest word
// and the longest word in it. If more than two words shared
// the same shortest or longest length, the first one will be 
// returned.
int main() {

  FILE *ptr_file;

  char tmp[100];
  char *shortest;
  char *longest;

  int count = 0;
  int minLen = 0x7fffffff;
  int maxLen = -1;
  int ch;
  // Open the 'input_file.txt' in read(r) mode.
  ptr_file = fopen("input_file.txt", "r"); 
  
  // If there is no file or file to open, error will be thrown
  if (ptr_file == NULL) {
   perror ("there is an error in opening the file");
   return -1;
  }
          
  printf("The content of the file: \n");

  while(1) {
    // Get next char from the stream
    ch = fgetc(ptr_file);
 
    //if the char is a letter, length counter will increment by 1,
    //and the char will be written to the next position of the 
    //temporary string
    if (ch >= 65 && ch <= 90 || ch >= 97 && ch <= 122) {
        tmp[count] = ch;     
        count++;
    } else {
      // if the counter is not 0 and it is smaller than the min length
      // update the min length.
      if (count != 0 && count < minLen) {
         minLen = count;
	 shortest = (char*)malloc(sizeof(char) * (minLen + 1));
	 strncpy(shortest, tmp, minLen);
	 shortest[minLen] = '\0';
      }
      // if the counter is not 0 and it is larger than the max length
      // update the max length
      if (count != 0 && count > maxLen) {
        maxLen = count;
	longest = (char*)malloc(sizeof(char) * (maxLen + 1));
	strncpy(longest, tmp, maxLen);
	longest[maxLen] = '\0'; 
      }
      // reset the counter to 0
      count = 0;
    } 
    
    // if reaching to the end of the file, we stop
    if (feof(ptr_file)) {
      break;
    }

    printf("%c", ch);
  }
  printf("\n");
  printf("The shortest string in the file is: %s.\n", shortest);
  printf("The longest string in the file is: %s.\n", longest);

  fclose(ptr_file); 
  return 0;
}
