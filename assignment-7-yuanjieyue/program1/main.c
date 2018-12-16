#include<stdio.h>
#include "factorial.h"
#include "fibonacci.h"

// The main function to test factorial.c and fibonacci.c
int main() {
  const int test = 10;
  printf("The factorial of %d is %d.\n", test, factorial(test));
  printf("The %dth fibonacci number is %d.\n", test, fib(test));
  return 0;
}
