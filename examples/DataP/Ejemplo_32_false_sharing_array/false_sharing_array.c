// Copyright 2021 Allan Berrocal <allan.berrocal@ucr.ac.cr > and
// Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0.
// Adapted from Mr. Jainam M https://github.com/MJjainam/falseSharing

#define _DEFAULT_SOURCE

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const size_t ELEMENT_COUNT = 1000;
const size_t FIRST_ELEMENT = 0;
const size_t SECOND_ELEMENT = 1;
const size_t LAST_ELEMENT = ELEMENT_COUNT - 1;
const size_t ITERATION_COUNT = 100000000;

const char* description[] = {
  "sequential(first, second)",
  "sequential(first, last)",
  "concurrent(first, second)",
  "concurrent(first, last)",
};

int* array = NULL;

// Updates an element of the array in a suboptimal manner
void *update_element(void *data) {
  const size_t my_index = (size_t)data;

  for (size_t iteration = 0; iteration < ITERATION_COUNT; ++iteration) {
    array[my_index] += iteration % ELEMENT_COUNT;
  }

  return NULL;
}

void run_sequential(size_t index0, size_t index1) {
  update_element((void*) index0);
  update_element((void*) index1);
}

void run_concurrent(size_t index0, size_t index1) {
  pthread_t thread0, thread1;
  pthread_create(&thread0, NULL, update_element, (void*) index0);
  pthread_create(&thread1, NULL, update_element, (void*) index1);
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
}

int main(int argc, char* argv[]) {
  const int mode = argc >= 2 ? atoi(argv[1]) : 0;
  array = (int*) calloc(ELEMENT_COUNT, sizeof(int));
  assert(array);

  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_REALTIME, &start_time);

  switch (mode) {
    case 0: run_sequential(FIRST_ELEMENT, SECOND_ELEMENT); break;
    case 1: run_sequential(FIRST_ELEMENT, LAST_ELEMENT); break;
    case 2: run_concurrent(FIRST_ELEMENT, SECOND_ELEMENT); break;
    case 3: run_concurrent(FIRST_ELEMENT, LAST_ELEMENT); break;
  }

  clock_gettime(CLOCK_REALTIME, &finish_time);
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  if (mode >= 0 && mode < 4) {
    printf("%.6lfs: %s\n", 1000 * elapsed_time, description[mode]);
  }

  free(array);
  return EXIT_SUCCESS;
}
