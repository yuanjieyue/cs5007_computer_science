#include <stdio.h>

// This is an interactive application does simple "mad libs".
int main() {

  const int dialogueNums = 4;

  char str[4][30];

  int i = 0;
  // Interaction loop for 'dialogueNums' time.
  while (i < dialogueNums) {
    switch (i) {
      case 0:
	      printf("Give me an noun: \n");
	      break;
      case 1:
	      printf("Give me a color: \n");
	      break;
      case 2: 
	      printf("Give me a city in Washington: \n");
	      break;
      case 3: 
	      printf("Give me a noun: \n");
	      break;
      default: 
              break;	      
    }
    // Read in an input.
    scanf("%s", str[i]);
    printf("Enter: %s\n", str[i]);   
    i++;
  }
  printf("The %s really want a %s kite.", str[0], str[1]);
  printf(" So it walked to %s but they were sold out", str[2]);
  printf(", so it bought a(or an) %s instead.\n", str[3]);
  return 0; 
}


