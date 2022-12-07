// Copyright 2021 Allan Berrocal <allan.berrocal@ucr.ac.cr > and
// Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0.
// Adapted from https://en.wikipedia.org/wiki/False_sharing

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const size_t ITERATION_COUNT = 100000000;

const char* description[] = {
  "sequential_shared",
  "sequential_private",
  "concurrent_shared",
  "concurrent_private",
};

typedef struct shared_data {
  size_t increment;
  size_t counter;
} shared_data_t;

// Fetches the value of increment from shared_data in every iteration even when
// its value does not change over time
void* run_shared_sum(void *data) {
  shared_data_t* shared_data = (shared_data_t*)data;

  size_t sum = 0;
  for (size_t iteration = 0; iteration < ITERATION_COUNT; ++iteration) {
    sum += shared_data->increment;
  }

  assert(sum == ITERATION_COUNT);
  return (void*) sum;
}

// Makes a local copy of increment from the shared data and uses it in every
// iteration of the for loop avoiding unnecesary fetching
void* run_private_sum(void *data) {
  shared_data_t* shared_data = (shared_data_t*)data;

  size_t sum = 0, local_increment = shared_data->increment;
  for (size_t iteration = 0; iteration < ITERATION_COUNT; ++iteration) {
    sum += local_increment;
  }

  assert(sum == ITERATION_COUNT);
  return (void*) sum;
}

// Updates counter of the shared data in a suboptimal manner to invalidate cache
void* increment_counter(void *data) {
  shared_data_t* shared_data = (shared_data_t*)data;

  for (size_t iteration = 0; iteration < ITERATION_COUNT; ++iteration) {
    shared_data->counter += 1;
  }

  assert(shared_data->counter == ITERATION_COUNT);
  return (void*) shared_data->counter;
}

void run_sequential_shared(shared_data_t* shared_data) {
  run_shared_sum(shared_data);
  increment_counter(shared_data);
}

void run_sequential_private(shared_data_t* shared_data) {
  run_private_sum(shared_data);
  increment_counter(shared_data);
}

void run_concurrent_shared(shared_data_t* shared_data) {
  pthread_t thread0, thread1;
  pthread_create(&thread0, NULL, run_shared_sum, shared_data);
  pthread_create(&thread1, NULL, increment_counter, shared_data);
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
}

void run_concurrent_private(shared_data_t* shared_data) {
  pthread_t thread0, thread1;
  pthread_create(&thread0, NULL, run_private_sum, shared_data);
  pthread_create(&thread1, NULL, increment_counter, shared_data);
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
}

int main(int argc, char* argv[]) {
  const int mode = argc >= 2 ? atoi(argv[1]) : 0;
  shared_data_t shared_data = {1, 0};

  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_REALTIME, &start_time);

  switch (mode) {
    case 0: run_sequential_shared(&shared_data); break;
    case 1: run_sequential_private(&shared_data); break;
    case 2: run_concurrent_shared(&shared_data); break;
    case 3: run_concurrent_private(&shared_data); break;
  }

  clock_gettime(CLOCK_REALTIME, &finish_time);
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
    (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  if (mode >= 0 && mode < 4) {
    printf("%.6lfms: %s\n", 1000 * elapsed_time, description[mode]);
  }

  return EXIT_SUCCESS;
}
