#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct shared_data {
  pthread_barrier_t* done_with_mapping;
  int* numbers;
  int data_amount;
  int worker_amount;
  int dynamic_mapping_counter;
  pthread_mutex_t can_access_counter;
} shared_data_t;

typedef struct private_data {
  int thread_num;
  shared_data_t* shared_data;
} private_data_t;

void read_arguments(int argc, char* argv[], int* worker_amount);
int read_numbers(int* numbers, int* number_amount);
void* work(void* data);
void serial_mapping(private_data_t* private_data, int* result);
void block_mapping(private_data_t* private_data, int* result);
void cyclic_mapping(private_data_t* private_data, int* result);
void dynamic_mapping(private_data_t* private_data, int* result);
int process_data(private_data_t* private_data, const int pos);
void free_resources(int* numbers);

#define MIN(a, b)\
  (a < b)? a : b

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  int worker_amount = sysconf(_SC_NPROCESSORS_ONLN);
  read_arguments(argc, argv, &worker_amount);

  int number_amount = 0;
  int* numbers = calloc(20, sizeof(int));

  if (numbers == NULL) {
    return error;
  }

  error = read_numbers(numbers, &number_amount);

  pthread_barrier_t done_with_mapping;
  pthread_barrier_init(&done_with_mapping, NULL, worker_amount);

  pthread_t* workers = calloc(worker_amount, sizeof(pthread_t));

  shared_data_t shared_data;
  shared_data.numbers = numbers;
  shared_data.done_with_mapping = &done_with_mapping;
  shared_data.data_amount = number_amount;
  shared_data.worker_amount = worker_amount;
  shared_data.dynamic_mapping_counter = 0;
  pthread_mutex_init(&shared_data.can_access_counter, NULL);

  private_data_t* private_data =
  calloc(worker_amount, sizeof(private_data_t));
  printf("begining to work: %i\n", worker_amount);

  for (int i = 0; i < number_amount; i++) {
    printf("%i,  ", numbers[i]);
  }

  for (int worker = 0; worker < worker_amount; worker++) {
    printf("worker %i created\n", worker);
    private_data[worker].shared_data = &shared_data;
    private_data[worker].thread_num = worker;
    pthread_create(&workers[worker], NULL,
    work, (void*) &private_data[worker]);
  }

  for (int worker = 0; worker < worker_amount; worker++) {
    int* results = NULL;
    pthread_join(workers[worker], (void*)&results);

    for (int i = 0; i < 3; i++) {
      printf("%i ", ((int*)results)[i]);
    }

    printf("\n");
  }

  free_resources(numbers);

  return error;
}

void read_arguments(int argc, char* argv[], int* worker_amount) {
  if (argc == 2) {
    sscanf(argv[1], "%i", worker_amount);
  }
}

int read_numbers(int* numbers, int* number_amount) {
  int error = EXIT_SUCCESS;
  int capacity = 20;

  while (scanf("%i", &numbers[*number_amount]) != 0) {
    (*number_amount)++;

    if (*number_amount == capacity) {
      capacity += 10;
      int* temp = realloc(numbers, capacity * sizeof(int));
      if (temp == NULL) {
        error = 50;
      } else {
        numbers = temp;
      }
    }
  }

  int* temp = realloc(numbers, *number_amount * sizeof(int));

  if (temp == NULL) {
    error = 50;
  } else {
    numbers = temp;
  }

  return error;
}

int get_start_pos(const int i, const int d, const int w) {
  int base = d/w;
  int minimum = MIN(i, d%w);
  int result = ((i * (base)) + minimum);
  return result;
}

void* work(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  int* results = calloc(3, sizeof(int));

  // block mapping
  block_mapping(private_data, results);

  // cyclic mapping
  cyclic_mapping(private_data, results);

  // dynamic mapping
  dynamic_mapping(private_data, results);

  printf("<%i, %i, %i>\n", results[0], results[1], results[2]);

  return (void*) (size_t)results;
}

void serial_mapping(private_data_t* private_data, int* result) {
  for (int position = 0;
  position < private_data->shared_data->data_amount;
  ++position) {
    (*result)++;
  }
}

void block_mapping(private_data_t* private_data, int* result) {
  int start_pos = get_start_pos(private_data->thread_num,
  private_data->shared_data->data_amount,
  private_data->shared_data->worker_amount);

  int stop_pos = get_start_pos(private_data->thread_num + 1,
  private_data->shared_data->data_amount,
  private_data->shared_data->worker_amount);

  for (int position = start_pos; position < stop_pos; ++position) {
    result[0] += process_data(private_data, position);
  } 

  pthread_barrier_wait(private_data->shared_data->done_with_mapping);
}

void cyclic_mapping(private_data_t* private_data, int* result) {
  for (int position = private_data->thread_num;
  position < private_data->shared_data->data_amount;
  position += private_data->shared_data->worker_amount) {
    result[1] += process_data(private_data, position);
  } 
  
  pthread_barrier_wait(private_data->shared_data->done_with_mapping);
}

void dynamic_mapping(private_data_t* private_data, int* result) {
  while(true) {
    pthread_mutex_lock(&private_data->shared_data->can_access_counter);
      int position = private_data->shared_data->dynamic_mapping_counter;
      (private_data->shared_data->dynamic_mapping_counter)++;
    pthread_mutex_unlock(&private_data->shared_data->can_access_counter);
    if (position >= private_data->shared_data->data_amount) {
      break;
    }
    result[2] += process_data(private_data, position);
  }
}

int process_data(private_data_t* private_data, const int pos) {
  int time_taken = private_data->shared_data->numbers[pos];

  printf("%i, %i\n", pos, time_taken);
  //usleep(time_taken);
  return time_taken;
}

void free_resources(int* numbers) {
  if (numbers != NULL) {
    //free(numbers);
  }
}