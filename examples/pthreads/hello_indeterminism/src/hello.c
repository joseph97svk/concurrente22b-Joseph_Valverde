// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
// #include <unistd.h>

/**
 * @brief declaration of function procedure
 * Displays greeting message
 */
void* greet(void* data);

// procedure main:
int main(int argc, char* argv[]) {
  // declare thread(greet)
  pthread_t thread;

  __int64_t thread_amount = sysconf(_SC_NPROCESSORS_ONLN);

  // if there are two arguments
  if (argc == 2) {
    if (sscanf(stdin, "%i", (__int64_t)thread_amount) == 1) {
      
    }  else {
      fprintf(stderr, "Error: invalid input given!");
      return EXIT_FAILURE;
    }
  } else if (argc == 1) {

  } else {
    fprintf(stderr, "Error: invalid input given!");
    return EXIT_FAILURE;
  }

  // create_thread(greet), parameter irrelevant
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
  
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");

    // join the thread, return value irrelevant
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    // in case a thread could not be spawned, report so
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
  // surpress unused parameter warning
  (void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");

  // must return a pointer, function is void*, not void
  return NULL;
}  // end procedure
