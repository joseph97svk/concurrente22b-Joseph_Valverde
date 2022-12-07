// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct shared_data {
  size_t team_count;
  useconds_t stage1_duration;
  useconds_t stage2_duration;
  size_t position;
} shared_data_t;

typedef struct private_data {
  size_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

int create_threads(shared_data_t* shared_data);
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);
void* start_race(void* data);
void* finish_race(void* data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      shared_data->position = 0;

      if (error == EXIT_SUCCESS) {
        struct timespec start_time, finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed_time);
      } else {
        fprintf(stderr, "error: could not init mutex\n");
        error = 11;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[]
    , shared_data_t* shared_data) {
  if (argc == 4) {
    if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1
      || shared_data->team_count == 0 ) {
      fprintf(stderr, "invalid team count: %s\n", argv[1]);
      return 11;
    }

    if ( sscanf(argv[2], "%u", &shared_data->stage1_duration) != 1 ) {
      fprintf(stderr, "invalid stage 1 duration: %s\n", argv[2]);
      return 12;
    }

    if ( sscanf(argv[3], "%u", &shared_data->stage2_duration) != 1 ) {
      fprintf(stderr, "invalid stage 2 duration: %s\n", argv[3]);
      return 13;
    }
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "usage: relay_race teams stage1duration stage2duration\n");
    return 10;
  }
}

#define ordered

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;

  const size_t thread_count = 2 * shared_data->team_count;
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));

  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));

  if (threads && private_data) {

    #ifdef ordered
    for (size_t index = 0; index < shared_data->team_count; ++index) {
    #endif
    #ifndef ordered
    for (size_t index = shared_data->team_count + 1; index >= 0 ; --index) {
    #endif
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, start_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }
    #ifdef ordered
    for (size_t index = shared_data->team_count; index < thread_count;
        ++index) {
    #endif
    #ifndef ordered
    for (size_t index = shared_data->team_count + 1; index >= 0 ; --index) {
    #endif
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, finish_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_data->team_count);
    error = 22;
  }

  return error;
}

void* start_race(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  usleep(1000 * shared_data->stage1_duration);

  return NULL;
}

void* finish_race(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  const size_t team_number = private_data->thread_number
    - shared_data->team_count;
  assert(team_number < shared_data->team_count);

  usleep(1000 * shared_data->stage2_duration);

  const size_t our_position = ++shared_data->position;
  printf("Place %zu: team %zu\n", our_position, team_number);

  return NULL;
}
