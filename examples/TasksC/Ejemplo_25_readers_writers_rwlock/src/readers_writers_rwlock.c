// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#define _DEFAULT_SOURCE

#include <assert.h>
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct shared_data {
  size_t counter;
} shared_data_t;

void* reader(void* data);
void* writer(void* data);
int create_threads(shared_data_t* shared_data);

int main() {
  int error = EXIT_SUCCESS;
  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    if (error == EXIT_SUCCESS) {
      error = create_threads(shared_data);
    } else {
      fprintf(stderr, "error: could not create rwlock\n");
      error = 11;
    }

    free(shared_data);
  } else {
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}

int resize(void** array, size_t* capacity, const size_t count, size_t elem_sz) {
  if (count < *capacity) {
    return EXIT_SUCCESS;
  }
  const size_t new_capacity = 10 * (*capacity ? *capacity : 1);
  void* new_arr = realloc(*array, new_capacity * elem_sz);
  if (new_arr == NULL) {
    return EXIT_FAILURE;
  }
  *capacity = new_capacity;
  *array = new_arr;
  return EXIT_SUCCESS;
}

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  size_t capacity = 0;
  size_t count = 0;
  pthread_t* threads = NULL;
  char ch = '\0';
  while (scanf("%c", &ch) == 1) {
    error = resize((void**)&threads, &capacity, count, sizeof(pthread_t));
    if (error) {
      fprintf(stderr, "error: could not allocate memory for threads\n");
      error = 22;
      break;
    }

    if (toupper(ch) == 'R') {
      error = pthread_create(&threads[count++], NULL, reader, shared_data);
    } else if (toupper(ch) == 'W') {
      error = pthread_create(&threads[count++], NULL, writer, shared_data);
    }  // Silently ignoring other chars

    if (error) {
      fprintf(stderr, "error: could not create thread %zu\n", count);
      error = 21;
      break;
    }
  }

  for (size_t index = 0; index < count; ++index) {
    pthread_join(threads[index], NULL);
  }

  free(threads);
  return error;
}

void* reader(void* data) {
  shared_data_t *shared_data = (shared_data_t*)data;

  size_t value = shared_data->counter;
  printf("Reader got %zu\n", value);
  return NULL;
}

void* writer(void* data) {
  shared_data_t *shared_data = (shared_data_t*)data;

  size_t value = ++shared_data->counter;
  printf("Writer increased to %zu\n", value);
  return NULL;
}
