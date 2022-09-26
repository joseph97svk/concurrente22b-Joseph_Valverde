// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include "Goldbach_arr.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief struct holding a number and its sums
 * @details Holds:
 * The number
 * The amount of sums
 * The array holding the sums
 */
typedef struct goldbach_element {
  int64_t number;  // the number
  int64_t sum_amount;  // amount of sums found
  int64_t** sums;  // 2D array of the sums
} goldbach_element_t;

/**
 * @brief Dynamic array holding goldbach_elements
 * @see goldbach_arr()
 */
struct goldbach_arr {
  int64_t capacity;   // amount of space available
  int64_t count;  // amount of contents
  int64_t total_sums_amount;  // sums between all elements in arr
  goldbach_element_t* elements;  // the elements with their numbers
};

/**
 * @brief Initializes a goldbach_element
 * 
 * @param element the element to be initialized
 * @return int32_t 
 */
int32_t goldbach_element_init(goldbach_element_t* element);

/**
 * @brief Deletes the goldbach element 
 * @details frees allocated memory for the given element
 * @param element element to be deleted
 */
void delete_goldbach_element(goldbach_element_t* element);

/**
 * @brief 
 * @see golbach_arr_create()
 */
goldbach_arr_t* goldbach_arr_create() {
  // allocate
  goldbach_arr_t* new_goldbach_arr = malloc(sizeof(goldbach_arr_t));

  if (new_goldbach_arr != NULL) {
    // initialize
    new_goldbach_arr -> count = 0;
    new_goldbach_arr -> capacity = 10;
    new_goldbach_arr -> total_sums_amount = 0;
    new_goldbach_arr -> elements =
    calloc(new_goldbach_arr -> capacity,
    sizeof(goldbach_element_t));

    /** if memory for elements could not be allocated, the goldbach arr creation
     * was not succesful, then free allocated space and return null
     */
    if (new_goldbach_arr -> elements == NULL) {
      free(new_goldbach_arr);
      new_goldbach_arr = NULL;
    }
  }

  return new_goldbach_arr;
}

/**
 * Initializes a goldbach_element
 * 
 */
int32_t goldbach_element_init(goldbach_element_t* element) {
  // initialize values
  element -> number = 0;
  element -> sum_amount = 0;
  element -> sums = NULL;  // get an address

  return EXIT_SUCCESS;
}


/**
 * Deletes the goldbach element 
 */
void delete_goldbach_element(goldbach_element_t* element) {
  free(element -> sums);
}

/**
 * Adds a number to dynamic array goldbach_arr
 * 
 */
int32_t goldbach_add_num(goldbach_arr_t* arr, const int64_t num) {
  // check if the memory had already been allocated
  if (arr -> elements == NULL) {
    arr = goldbach_arr_create();
  } else if (arr -> capacity == arr -> count) {
    // increase the count
    arr -> capacity = arr -> capacity + 10;

    // reallocate memory according to new capacity
    arr -> elements = realloc(arr -> elements,
    ((arr -> capacity)) * sizeof (goldbach_element_t));
  }

  // if everything failed with memory allocation
  if (arr -> elements == NULL) {
    // erase all done and report failure
    goldbach_arr_destroy(arr);
    return error_goldbach_arr_elements_memory_allocation_failure;
  }

  // initialize an element for the given number
  goldbach_element_init(&(arr -> elements)[arr -> count]);

  // set the number
  (arr -> elements)[arr -> count].number = num;

  // increase the count of the array
  arr -> count = (arr -> count) + 1;

  return EXIT_SUCCESS;
}

/**
 * @brief checks for valid positioning and null pointers
 * 
 * @param arr goldbach_arr
 * @param sum the sum to be added to the array
 * @param position position of the number whose sum is being added
 * @return int32_t 
 */
int32_t check_sum_params(goldbach_arr_t* arr,
const int64_t* const sum, const int64_t position) {
  // check is parameters are correct
  if (position >= arr -> count|| sum == NULL) {
    return error_adding_sum_failure_invalid_position_or_given_sum;
  }

  // check if memory had previously been allocated
  if (arr -> elements[position].sums == NULL) {
    arr -> elements[position].sums = malloc(sizeof(int64_t**) * 10);
  }

  // if memory could not be allocated previously then report failure
  if (arr -> elements[position].sums == NULL) {
    return error_adding_sum_sums_memory_allocation_failure;
  }

  return EXIT_SUCCESS;
}

/**
 * @brief increases the capacity if needed and the related counters
 * 
 * @param arr golbach_arr
 * @param position of the number whose sum is being added
 * @return int32_t 
 */
int32_t increase_sums_size(goldbach_arr_t* arr, const int64_t position) {
  // increase the amount of space in the array of pointers
  if (arr -> elements[position].sum_amount != 0 &&
  arr -> elements[position].sum_amount % 10 == 0) {
    int64_t** sums_buffer = realloc(arr -> elements[position].sums,
    ((arr -> elements[position].sum_amount) + 10) * sizeof(int64_t**));

    // if memory for sums could not be allocted, report failure
    if (sums_buffer == NULL) {
      return error_adding_sum_current_sum_memory_allocation_failure;
    }

    // assign new address if succesful
    arr -> elements[position].sums = sums_buffer;
  }

  // increase sum amount
  arr -> elements[position].sum_amount =
  (arr -> elements[position].sum_amount) + 1;

  return EXIT_SUCCESS;
}

/**
 * @brief copies the sum from the given array
 * into the goldbach_arr
 * 
 * @param arr golbach_arr
 * @param sum sum being added
 * @param position of the number whose sum is being added
 * @param sum_position position in the array for a given number
 * @return int32_t 
 */
int32_t copy_sum(goldbach_arr_t* arr,
const int64_t* const sum, const int64_t position,
const int64_t sum_position) {
  // buffer the sum amount
  int64_t sum_element_amount = 3;

  // check the lenght of the sum
  if (arr -> elements[position].number % 2 == 0) {
      sum_element_amount = 2;
  }

  // allocate the space in the given pointer
  arr -> elements[position].sums[sum_position] =
  malloc(sum_element_amount * sizeof(int64_t));

  // if memory for a given sum could not be allocated, report failure
  if (arr -> elements[position].sums[sum_position] == NULL) {
    return error_adding_sum_current_sum_memory_allocation_failure;
  }

  const int64_t* pointer_check = sum;

  // for all numbers  in a sum
  for (int32_t sum_element = 0;
  sum_element < sum_element_amount; sum_element++) {
    // check if the pointer was correctly allocted
    if (pointer_check == NULL) { // NOLINT
      return error_adding_sum_provided_sum_invalid_size;
    }

    // add the numbers from the given sum to the element sums array
    arr -> elements[position].sums[sum_position][sum_element] =
    sum[sum_element];
    pointer_check += (sizeof(int64_t*));
  }

  return EXIT_SUCCESS;
}

/**
 * Adds an array of the numbers of an addition
 * Copies numbers from given array into arr,
 * no need to dinamically allocate and given arr can be discarded
 * Copies amount of numbers according to number in position of arr
 */
int32_t goldbach_add_sum(goldbach_arr_t* arr,
const int64_t* const sum, const int64_t position) {
  int32_t error = EXIT_SUCCESS;

  // make sure everything is ready
  check_sum_params(arr, sum, position);

  // get the position for the operations
  int64_t sum_position = arr -> elements[position].sum_amount;

  // increase the space for the sums to be added
  error = increase_sums_size(arr, position);

  if (error != EXIT_SUCCESS) {
    return error;
  }

  // copy the sum into the array
  error = copy_sum(arr, sum, position, sum_position);

  if (error != EXIT_SUCCESS) {
    return error;
  }

  // increase sums counter
  (arr -> total_sums_amount) += 1;
  return error;
}

/**
 * increases a number sum count without adding sums
 * 
 */
void goldbach_add_ghost_sum(goldbach_arr_t* arr, const int64_t position) {
  (arr -> elements[position].sum_amount) += 1;
  (arr -> total_sums_amount) += 1;
}

/**
 * removes extra capacity allocated for numbers
 * 
 */
int32_t goldbach_finish_num_sums(goldbach_arr_t* arr, const int64_t position) {
  if (arr -> elements[position].number > 0) {
    return EXIT_SUCCESS;
  }

  int64_t** buffer = realloc(arr -> elements[position].sums,
  sizeof(int64_t**) * arr -> elements[position].sum_amount);

  if (buffer == NULL) {
    return error_adding_sum_sums_memory_allocation_failure;
  }

  arr -> elements[position].sums = buffer;
  return EXIT_SUCCESS;
}

/**
 * returns the count of elements or numbers in arr
 */
int64_t goldbach_get_arr_count(goldbach_arr_t* arr) {
  if (arr == NULL) {
    return -1;
  }

  return arr -> count;
}

/**
 * Returns the number at a given position
 * invalid position will return 0
 */
int64_t goldbach_get_current_number(goldbach_arr_t* arr,
const int64_t position) {
  if (position < 0 || position > arr -> count) {
    return 0;
  }
  return arr -> elements[position].number;
}

/**
 * returns the amount of sums located in a given position of arr
 */
int64_t goldbach_get_sums_amount(goldbach_arr_t* arr, const int64_t position) {
  if (position < 0 || position > arr -> count) {
    return -1;
  }

  return arr -> elements[position].sum_amount;
}

/**
 * returns the total amount of sums stored among all the numbers in the array
 * 
 */
int64_t goldbach_arr_get_total_sums_amount(goldbach_arr_t* arr) {
    return arr -> total_sums_amount;
}

/**
 * Returns an array with the numbers of the sum
 * sum must be dealocated afterwards to prevent memory leak
 * invalid positions return null pointer
 * failure to allocate array for sum will return null pointer
 */
int64_t* goldbach_get_sum(const goldbach_arr_t* const arr, int64_t* size,
const int64_t num_position, const int64_t sum_position) {
  // if out of bounds, then return null pointer
  if (num_position > arr -> count) {
    return NULL;
  }

  /* if out of bounds, then return null pointer.
  After previous check to prevent undefined behaviour
  */
  if (sum_position > arr -> elements[num_position].sum_amount) {
    return NULL;
  }

  // return the size to a pointer
  *size = 3;

  if (arr -> elements[num_position].number % 2 == 0) {
    *size = 2;
  }

  // allocate space for the array to be returned
  int64_t* sum = malloc(*size * sizeof(int64_t));

  // if sum could not be allocated, will return null
  if (sum == NULL) {
    return NULL;
  }

  // copy the sum to the allocated array to be returned
  for (int64_t sum_num = 0; sum_num < *size; sum_num++) {
    if (sum != NULL ||
    arr -> elements[num_position].sums != NULL) {
      sum[sum_num] =
      arr -> elements[num_position].sums[sum_position][sum_num];
    }
  }

  return sum;
}

/**
 * deallocates all memory allocated for goldbach_arr
 */
void goldbach_arr_destroy(goldbach_arr_t* arr) {
  // for all elements in the array
  for (int64_t element = 0; element < arr -> count; element++) {
    // for all sums in the a given element
    if (arr -> elements[element].number < 0) {
      for (int64_t sum = 0;
      sum < arr -> elements[element].sum_amount; sum++) {
        // free if previously correctly allocated
        if (arr -> elements[element].sums[sum] != NULL) {
          free(arr -> elements[element].sums[sum]);
        }
      }

      // free the sums array
      if (arr -> elements[element].sums != NULL) {
        free(arr -> elements[element].sums);
      }
    }
  }

  // free the elements array
  if (arr -> elements != NULL) {
      free(arr -> elements);
  }

  // free the space for the goldbach_arr
  if (arr != NULL) {
      free(arr);
  }
}
