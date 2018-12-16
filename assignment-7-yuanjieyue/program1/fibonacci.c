#include<stdio.h>

// Calculate the numTH fibonacci number.
// The num should be equals or larger than 0.
int fib(int num) {
  // The base case.
  if (num <= 1) {
    return num;	
  }
  // When the num is greater than 1, do recursion
  else {
   return fib(num - 1) + fib(num - 2);
  } 
}

