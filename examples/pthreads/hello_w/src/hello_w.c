// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <inttypes.h>
// #include <unistd.h>

/**
 * @brief declaration of function procedure
 * Displays greeting message
 */
void* greet(void* data);

int create_threads (uint64_t thread_count);

// procedure main:
int main(int argc, char* argv[]) {
  int64_t thread_amount = sysconf(_SC_NPROCESSORS_ONLN);

  // if there are two arguments
  if (argc == 2) {
    if (sscanf(argv[1], "%li", &thread_amount) != 1) {
      fprintf(stderr, "Error: invalid input given!\n");
      return EXIT_FAILURE;
    }
  } else if (argc > 2) {
    fprintf(stderr, "Error: invalid input given! Too many arguments!\n");
    return EXIT_FAILURE;
  } 

  // create_threads
  return create_threads (thread_amount);
  
}  // end procedure

int create_threads (uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  //int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);

  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  
  if (threads != NULL) {
    for (uint64_t current_thread = 0; current_thread < thread_count;
     ++current_thread) {
       error = pthread_create(&threads[current_thread], NULL, greet,
       (void*) current_thread);

      if (error != EXIT_SUCCESS) {
        fprintf(stderr, "Error: could not create secondary thread\n");
        break;
      }
    }

    printf("Hello from main thread!\n");

    for (uint64_t current_thread = 0; current_thread < thread_count;
     ++current_thread) {
      pthread_join(threads[current_thread], NULL);
    }

    free(threads);
  } else {
    // in case a thread could not be spawned, report so
    fprintf(stderr, "Error: could not create given thread amount\n");
  }
  return error;
}

// procedure greet:
void* greet(void* data) {
  const uint64_t rank = (uint64_t) data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %lu\n", rank);

  // must return a pointer, function is void*, not void
  return NULL;
}  // end procedure
