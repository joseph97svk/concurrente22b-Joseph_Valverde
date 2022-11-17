// Copyright [2022] <Joseph Valverde, Alvaro Arguedas, Carlos Mora>
#ifndef GOLDBACHCONJECTURESTRUCTURE_H
#define GOLDBACHCONJECTURESTRUCTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef int64_t unit_t;

typedef struct goldbach_arr goldbach_arr_t;

typedef struct goldbach_conjecture {
  int64_t* numbers;
  int64_t number_count;
  int64_t number_capacity;
  goldbach_arr_t* goldbach_arr;
  int32_t thread_amount;
  unit_t max_value;
  bool* prime_number_list;
  int64_t prime_number_list_capacity;
} goldbach_conjecture_t;

typedef struct goldbach_data {
  goldbach_arr_t* goldbach_arr;
  unit_t max_value;
  bool* prime_number_list;
  int64_t prime_number_list_capacity;
} goldbach_data_t;

#endif

