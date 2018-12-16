#include<stdio.h>

// To calculate the factorial of the given num. 
// num should be equals or larger than 1.
int factorial(int num) {
  // The base case.	
  if (num == 1) {
    return 1;
  } 
  // When num is greater than 1, do the recursion.
  else { 
    return num * factorial(num - 1);
  }
}

