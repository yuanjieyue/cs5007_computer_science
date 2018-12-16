#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Input an original string, a substring in the original one that needed to
// be replaced, and the substitute string to replace.
// Return a original string with all the given substring replaced by the given
// substitute string.
char const* replace(char const* const original, char const* const toBeReplaced,
		    char const* const substitute) {
  // Get the length of there given strings.
  size_t const oriLen = strlen(original);
  size_t const toBeRepLen = strlen(toBeReplaced);
  size_t const subsLen = strlen(substitute);
  
  size_t numOfToBeReplaced = 0;
  char const* oriPtr;
  char const* toBeRepLoc;
  
  // Find how many times that the toBeReplaced occurs in the original string.
  for (oriPtr = original; toBeRepLoc = strstr(oriPtr, toBeReplaced);
       oriPtr = toBeRepLoc + toBeRepLen) {
    numOfToBeReplaced++;
  }
  // Allocate memory for the result string.
  size_t const resLen = oriLen + numOfToBeReplaced * (subsLen - toBeRepLen);
  char* const res = (char*)malloc(sizeof(char) * (resLen + 1));
  
  if (res != NULL) {
    char* resPtr = res;
    for (oriPtr = original; toBeRepLoc = strstr(oriPtr, toBeReplaced);
	 oriPtr = toBeRepLoc + toBeRepLen) {
      // Get the length between the head and the toBeReplaced location.
      size_t const skpLen = toBeRepLoc - oriPtr;
      // Copy the characters before the next toBeReplaced to the new string.
      strncpy(resPtr, oriPtr, skpLen);
      resPtr += skpLen;
      // Copy the substitute characters to the new string instead of the
      // toBeReplaced one.
      strncpy(resPtr, substitute, subsLen);
      resPtr += subsLen;
    }
    // Copy the rest part of the original string to the new string.
    strcpy(resPtr, oriPtr);
  }
  return res;
}







