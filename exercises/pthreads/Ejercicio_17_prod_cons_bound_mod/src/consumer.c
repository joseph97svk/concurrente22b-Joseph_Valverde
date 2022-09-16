#include "consumer.h"
#include "thread_data.h"

void* consume(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)data;
  for (size_t round = 0; round < shared_data->rounds; ++round) {
    for (size_t index = 0; index < shared_data->buffer_capacity; ++index) {
      // wait(can_consume)
      sem_wait(&shared_data->can_consume);

      double value = shared_data->buffer[index];
      usleep(1000 * random_between(shared_data->consumer_min_delay
        , shared_data->consumer_max_delay));
      printf("\tConsumed %lg\n", value);

      // signal(can_produce)
      sem_post(&shared_data->can_produce);
    }
  }

  return NULL;
}

