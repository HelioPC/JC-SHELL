/*
// Fibonacci, version 2
// Sistemas Operativos, DEI-CC/FC/UAN 2020
// Professor Joao Costa
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define NORM "\033[m"
#define GREEN "\033[32m"
#define D_BLUE "\033[34m"
#define RED "\033[31m"
#define NUM_DUMMY_ITERATIONS 100000

int main (int argc, char** argv) {

  unsigned long n, first = 0, second = 1, next, c, caux;
  
  if (argc != 2) { 
    printf("%sIncorrect arguments:%s fibonacci number_of_terms\n", RED, NORM);
    exit(EXIT_FAILURE);
  }
  
  n = atoi(argv[1]);
  
  printf("%sProcess %d started to generate Fibonacci up to %lu%s.\n", GREEN,
        (int) getpid(), n, NORM);
 
  for ( c = 0 ; c < n ; c++ )
  {
    for (caux = 0; caux < NUM_DUMMY_ITERATIONS; caux ++) ;
    
    if ( c <= 1 )
      next = c;
    else
      {
	next = first + second;
	first = second;
	second = next;
      }
  }
  
  printf("%sProcess %d finished Fibonacci series at value: %lu%s.\n", D_BLUE,
        (int) getpid(), next, NORM);
 
  exit(EXIT_SUCCESS);
}
