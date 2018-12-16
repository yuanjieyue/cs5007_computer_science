#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Input the original string that to be reversed.
// Output the original string in reverse order.
char const* reverse_words(char const* original) {
  // Get the length of the original string.
  size_t const oriLen = strlen(original);

  // Malloc memory for the result string.
  char* const res = (char*)malloc(sizeof(char) * (oriLen + 1));

  char const* oriPtr;
  char* resPtr;

  // Loop over the original string from right to left,
  // and copy the character to the result string from left to right.
  for (oriPtr = original + oriLen - 1, resPtr = res; oriPtr >= original;
       oriPtr--, resPtr++) {
    strncpy(resPtr, oriPtr, 1);
  }

  return res;
}
