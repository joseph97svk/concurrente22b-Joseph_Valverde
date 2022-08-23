// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief ...
 */
void* greet(void* data);

// procedure main:
int main(void) {
  // create_thread(greet)
  pthread_t thread;

  // initial val for the recursive
  size_t initial_val = 2;

  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ (void*)&initial_val);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    //return value irrelevant
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
  //void pointer >> size_t pointer >> size_t >> assign to local variable
  const size_t action = *((size_t*) data);
  int recursive_hello_error = 0;

  // if action is more than 0
  if (action > 0) {
    // greet
    printf("Hello there: %lu\n", action);


    // create thread and buffer with new value
    pthread_t recursive_hello_thread;
    size_t buffer =  action - 1;    

    // recursive
    recursive_hello_error = pthread_create(&recursive_hello_thread, /*attr*/ NULL, greet, /*arg*/ (void*)&buffer);

    // add thread only if one was created
    if (recursive_hello_error == EXIT_SUCCESS) {
      // return value irrelevant
      pthread_join(recursive_hello_thread, NULL);
    }

  // for the other case say goodbye
  } else {
    printf("Tschuss: %lu\n", action);
  }

  // if at any point there was an error, report so 
  if (recursive_hello_error != EXIT_SUCCESS) {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  
  //(void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");
  return NULL;
}  // end procedure
