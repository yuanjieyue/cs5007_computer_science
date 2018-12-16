#include<stdio.h>

extern char const* replace(char const*, char const*, char const*);
extern char const* reverse_words(char const*);

// Here is the main function to run the replace.c and reverse_words.c
int main() {
  char const* pre_defined_string = "The brown fox jumped over the lazy dog.";
  printf("%s\n", pre_defined_string);
  
  char const* toBeReplaced = "brown";
  char const* substitute = "yellow";
  char const* second_string = replace(pre_defined_string, toBeReplaced, substitute);
  printf("%s\n", second_string);


  char const* reverse = reverse_words(pre_defined_string);
  printf("%s\n", reverse);

  return 0;
}
