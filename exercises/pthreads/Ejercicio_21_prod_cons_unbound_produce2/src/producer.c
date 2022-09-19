// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdio.h>

#include "common.h"
#include "producer.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = (simulation_t*)data;

  while (true) {
    // runs until semaphore reaches 0, at which case count runs out
    if (sem_trywait(&simulation -> can_produce)) {
      sem_post(&simulation->can_consume);
      break;
    }

    usleep(1000 * random_between(simulation->producer_min_delay
      , simulation->producer_max_delay));

    pthread_mutex_lock(&simulation->can_access_queue);
      // declare my_unit := get_count(queue) + 1
      size_t my_unit = queue_get_count(&simulation -> queue) +
      (simulation -> consumed_count) + 1;

      queue_enqueue(&simulation->queue, my_unit);
    pthread_mutex_unlock(&simulation->can_access_queue);

    printf("Produced %zu\n", my_unit);

    // signal(can_consume)
    sem_post(&simulation->can_consume);
  }

  return NULL;
}
