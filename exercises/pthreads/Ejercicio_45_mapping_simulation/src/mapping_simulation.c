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
int* read_numbers(int* number_amount);
void* work(void* data);
int serial_mapping(shared_data_t* shared_data);
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

  int* numbers = read_numbers(&number_amount);

  if (numbers == NULL) {
    return error;
  }

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

  for (int worker = 0; worker < worker_amount; worker++) {
    private_data[worker].shared_data = &shared_data;
    private_data[worker].thread_num = worker;
    pthread_create(&workers[worker], NULL,
    work, (void*) &private_data[worker]);
  }

  int** results = calloc(worker_amount, sizeof(int*));

  for (int worker = 0; worker < worker_amount; worker++) {
    pthread_join(workers[worker], (void*)&results[worker]);
  }

  int previous = serial_mapping(&shared_data);

  printf("%i\t ", previous);

  for (int worker = 0; worker < worker_amount; worker++) {
    printf("%i\t", worker);
  }

  printf("Duration\tSpeedUp\t\tEfficiency\n");

  for (int mapping = 0; mapping < 3; ++mapping) {
    switch (mapping)
    {
    case 0:
      printf("Block \t ");
      break;
    case 1:
      printf("Cyclic \t ");
      break;
    case 2:
      printf("Dynamic \t ");
      break;
    }

    int duration = 0;
    for (int worker = 0; worker < worker_amount; worker++) {
      printf("%i\t", results[worker][mapping]);
      if (results[worker][mapping] > duration) {
        duration = results[worker][mapping];
      }
    }

    double speedup = previous/duration, 
    efficiency = speedup/worker_amount;

    printf("%i\t\t", duration);
    printf("%f\t", speedup);
    printf("%f\t", efficiency);


    printf("\n");
  }

  for (int worker = 0; worker < worker_amount; worker++) {
    free(results[worker]);
  }

  free(results);

  free_resources(numbers);
  free(private_data);
  free(workers);

  return error;
}

void read_arguments(int argc, char* argv[], int* worker_amount) {
  if (argc == 2) {
    sscanf(argv[1], "%i", worker_amount);
  }
}

int* read_numbers(int* number_amount) {
  int* numbers = calloc(20, sizeof(int));
  int capacity = 20;
  int number_amount_local = 0;

  while (scanf("%i", &numbers[number_amount_local]) != 0) {
    number_amount_local++;

    if (number_amount_local == capacity) {
      capacity *= 10;
      int* temp = realloc(numbers, capacity * sizeof(int));
      if (temp == NULL) {
        return NULL;
      } else {
        numbers = temp;
      }
    }
  }


  int* temp = realloc(numbers, number_amount_local * sizeof(int));

  if (temp == NULL) {
    return NULL;
  } else {
    numbers = temp;
  }

  *number_amount = number_amount_local;

  return numbers;
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

  return (void*) (size_t)results;
}

int serial_mapping(shared_data_t* shared_data) {
  int result = 0;
  for (int position = 0;
  position < shared_data->data_amount;
  ++position) {
    result++;
  }
  return result;
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
  usleep(time_taken);
  return time_taken;
}

void free_resources(int* numbers) {
  if (numbers != NULL) {
    free(numbers);
  }
}