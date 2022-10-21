// Copyright [2022] <Joseph Valverde, Alvaro Arguedas, Carlos Mora>
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef int64_t unit_t;

typedef struct goldbach_arr goldbach_arr_t;

typedef struct goldbach_conjecture {
  goldbach_arr_t* goldbach_arr;
  pthread_t* threads;
  int32_t thread_amount;
  unit_t last_processed_position;
  pthread_mutex_t can_access_position;
  pthread_mutex_t can_set_prime_num;
  pthread_barrier_t done_with_primes;
  unit_t max_value;
  bool* prime_number_list;
  int64_t prime_number_list_capacity;
} goldbach_conjecture_t;
