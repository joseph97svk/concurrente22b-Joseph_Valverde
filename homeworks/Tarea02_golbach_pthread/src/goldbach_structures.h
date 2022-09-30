#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef int64_t unit_t;

/**
 * @brief struct holding a number and its sums
 * @details Holds:
 * The number
 * The amount of sums
 * The array holding the sums
 */
typedef struct goldbach_element {
  unit_t number;  // the number
  int64_t sum_amount;  // amount of sums found
  unit_t** sums;  // 2D array of the sums
} goldbach_element_t;

/**
 * @brief Dynamic array holding goldbach_elements
 * @see goldbach_arr()
 */
struct goldbach_arr {
  int64_t capacity;   // amount of space available
  int64_t count;  // amount of contents
  int64_t total_sums_amount;  // sums between all elements in arr
  void* elements;  // the elements with their numbers

  pthread_mutex_t can_access_array;
};