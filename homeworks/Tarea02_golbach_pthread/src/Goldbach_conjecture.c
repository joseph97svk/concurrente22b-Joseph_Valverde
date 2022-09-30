// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include "Goldbach_conjecture.h"

typedef struct goldbach_conjecture {
  goldbach_arr_t* goldbach_arr;
  pthread_t* threads;
  int32_t thread_amount;
  int64_t last_processed_position;
  pthread_mutex_t can_access_position;
} goldbach_conjecture_t;

/**
 * @brief checks if the given input is valid and gives the value
 * 
 * @param string to be checked
 * @param value where the value will be stored
 * @return true if value is valid
 * @return false if value is invalid
 */

bool num_validity_check(char string[64], int64_t* value);

/**
 * @brief checks the first char of a char string for its validity
 * 
 * @param string char string to be checked
 * @param minus_found amount of '-' found
 * @param num_found if what has been found so far is a number
 * @return true 
 * @return false 
 */
bool check_first_char(char string[64], int32_t* minus_found, bool* num_found);

/**
 * @brief Checks all characters in a string to check if they are valid
 * 
 * @param string char string to be checked
 * @param minus_found amount of '-' found
 * @param num_found if what has been found so far is a number
 * @return true 
 * @return false 
 */
bool check_all_chars(char string[64], int32_t* minus_found, bool* num_found);

/**
 * @brief Processess a given number according to the goldbach conjecture
 * @details Handles multi-threaded related operations
 * @param goldbach_arr where the results will be stored
 * @param number number to be processed
 * @param position position within the goldbach_arr of the number
 * @return int32_t 
 */

void* goldbach_process_num(void* data);

/**
 * @brief Processes operations to determine goldbach sums
 * 
 * @param goldbach_data 
 * @param position 
 * @return int32_t 
 */
int32_t num_golbach_process(goldbach_conjecture_t* goldbach_data,
const int64_t position);

/**
 * @brief processes an odd number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num
 * @param num @see goldbach_process_num
 * @param position @see goldbach_process_num
 * @return int32_t 
 */

int32_t goldbach_odd_process(goldbach_conjecture_t* goldbach_data,
const int64_t num, bool positive, const int64_t position);

/**
 * @brief processes an even number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num()
 * @param num @see goldbach_process_num()
 * @param position @see goldbach_process_num()
 * @return int32_t 
 */

int32_t goldbach_even_process(goldbach_conjecture_t* goldbach_data,
const int64_t num, bool positive, const int64_t position);

/**
 * @brief Finds the next prime number after the given number
 * @details last_prime may or may not be prime,
 * will still, regardless, find the next prime number
 * 
 * @param last_prime number after which the next prime number is to be found
 * @return int64_t 
 */

int64_t find_next_prime(const int64_t last_prime);

/**
 * @brief checks if a given number is prime
 * @details based on the primality test algorithm in
 * https://en.wikipedia.org/wiki/Primality_test
 * 
 * Based on the principle where all prime numbers greater than 3
 * can be representad as 6k + 1
 * @param number the number to be checked
 * @return true if the number is prime
 * @return false if the number is not prime
 */

bool isPrimeNum(const int64_t number);

/**
 * @brief Prints the amount of sums found for a given number
 * 
 * @param goldbach_arr
 * @param sum_amount amount of sums found for the number
 * @param current_num the actual number
 * @param num position of the number within the array
 */
void print_sums_amount(goldbach_arr_t* goldbach_arr, int64_t* sum_amount,
const int64_t current_num, const int num);

/**
 * @brief prints the actual sums found for a given number
 * 
 * @param goldbach_arr 
 * @param sum_amount amount of sums found for the number
 * @param current_sum temp array for the sum
 * @param size size of each sum
 * @param num position of the number in the array 
 */
void print_sums_for_num(goldbach_arr_t* goldbach_arr,
const int64_t sum_amount, int64_t* current_sum,
int64_t* size, const int64_t num);

/**
 * @brief returns a set up goldbach_arr based on the
 * arguments/paramenters
 * @param argc amount of arguments
 * @param argv arguments
 * @return golbach_conjecture_t* 
 */
goldbach_conjecture_t* goldbach_set_up(int argc, char* argv[]) {
  if (argc > 2) {
    return NULL;
  }

  goldbach_arr_t* goldbach_arr = goldbach_arr_create();

  if (goldbach_arr == NULL) {
    return NULL;
  }

  goldbach_conjecture_t* goldbach_conjecture =
  calloc(1, sizeof(goldbach_conjecture_t));

  if (goldbach_conjecture == NULL) {
    free(goldbach_arr);
    return NULL;
  }

  goldbach_conjecture->goldbach_arr = goldbach_arr;
  goldbach_conjecture->threads = NULL;
  goldbach_conjecture->thread_amount = sysconf(_SC_NPROCESSORS_ONLN);

  if (argc == 2) {
    int32_t thread_amount = 0;
    sscanf(argv[1], "%i", &thread_amount);
    goldbach_conjecture->thread_amount = thread_amount;
  }

  return goldbach_conjecture;
}

/**
 * @brief reads the input to the console and adds them to the 
 * goldbach_arr for later processing
 * @details DOES NOT SOLVE anything, just reads.
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers read 
 * will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_read_numbers(goldbach_conjecture_t* goldbach_conjecture) {
  // space to read a value up to 64 digits
  char input_read[64];
  int64_t current_val_read = 0;

  int32_t number_addition_error = EXIT_SUCCESS;

  // while numbers are being read
  while (fgets(input_read, sizeof(input_read), stdin) != NULL) {
    if (input_read[0] == '\n') {
      return EXIT_SUCCESS;
    }

    int32_t erase_space = strlen(input_read);

    // check if input is valid
    if (num_validity_check(input_read, &current_val_read)) {
      // if valid, add them
      number_addition_error =
      goldbach_add_num(goldbach_conjecture->goldbach_arr, current_val_read);

      if (number_addition_error != EXIT_SUCCESS) {
        return number_addition_error;
      }
    } else {
      // if not valid, report so
      return error_invalid_input_given;
    }

    // ensure all values on input space to be 0 for next run
    for (int32_t character = 0; character < erase_space; character++) {
      input_read[character] = '0';
    }
  }

  return number_addition_error;
}

/**
 * checks if the given input is valid and gives the value
 * 
 */
bool num_validity_check(char string[64], int64_t* value) {
  errno = 0;

  char* end_ptr;

  bool num_found = string[0] > 47 && string[0] < 58;
  int32_t minus_found = 0;

  if (!check_first_char(string, &minus_found, &num_found)) {
    return false;
  }

  if (!check_all_chars(string, &minus_found, &num_found)) {
    return false;
  }

  // change input to number
  int64_t number = strtol(string, &end_ptr, 10); //NOLINT

  // check if there was an overflow
  if (errno != 0) {
    return false;
  }

  *value = number;

  return true;
}

/**
 * Checks the first char of a char string for its validity
 * 
 */
bool check_first_char(char string[64], int32_t* minus_found, bool* num_found) {
  // if the first char is not a number
  if (!*num_found && string[0] != 32) {
    // if the first char is not num, and also not a minus
    if (string[0] != '-') {
      return false;
    // otherwise if is minus but is only that
    } else if (strlen(string) == 1) {
      return false;
    }
    (*minus_found)++;
  }

  return true;
}

/**
 * Checks all characters in a string to check if they are valid
 * 
 */
bool check_all_chars(char string[64], int32_t* minus_found, bool* num_found) {
  // begin checking at second char if it is num (first already checked)
  for (size_t character = 1; character < strlen(string) - 1; character++) {
    if (*minus_found > 1) {
      return false;
    }

    if (!*num_found) {
      if (string[character] == '-') {
         (*minus_found)++;
        if (string[character - 1] == '-') {
          return false;
        }
        continue;
      } else if (string[character] == 32 &&
      *minus_found > 0) {
        return false;
      }
    }

    *num_found = string[character] > 47 && string[character] < 58;

    // if subsequent chars are not nums
    if (!*num_found && string[character] != 32) { // NOLINT
      // then invalid
      return false;
    }
  }

  return true;
}

/**
 * Processes and solves the numbers according to the 
 * Golbach conjecture
 */
int32_t goldbach_process_sums(goldbach_conjecture_t* goldbach_conjecture) {
  int32_t num_process_error = EXIT_SUCCESS;

  goldbach_conjecture->threads =
  calloc(goldbach_conjecture->thread_amount, sizeof(pthread_t));

  if (goldbach_conjecture->thread_amount > 
  goldbach_get_arr_count(goldbach_conjecture->goldbach_arr)) {
    goldbach_conjecture->thread_amount = goldbach_get_arr_count(goldbach_conjecture->goldbach_arr);
  }

  goldbach_conjecture->last_processed_position = 0;

  pthread_mutex_init(&(goldbach_conjecture->can_access_position), NULL);

  // for all elements/numbers in a goldbach_arr
  for (int64_t number = 0;
  number < goldbach_conjecture->thread_amount;
  number++) {
    // process each given number
    num_process_error =
    pthread_create(&goldbach_conjecture->threads[number], NULL,
    goldbach_process_num, (void*)goldbach_conjecture);

    if (num_process_error != EXIT_SUCCESS) {
      free(goldbach_conjecture->threads);
      return num_process_error;
    }
  }

  // join threads
  for (int64_t number = 0;
  number < goldbach_conjecture->thread_amount;
  number++) {
    pthread_join(goldbach_conjecture->threads[number],
    (void*) &num_process_error);
  }

  free(goldbach_conjecture->threads);

  return num_process_error;
}

/**
 * Processess a given number according to the goldbach conjecture
 * Mostly thread related 
 */
void* goldbach_process_num(void* data) {
  goldbach_conjecture_t* goldbach_data =
  (goldbach_conjecture_t*) data;

  goldbach_arr_t* goldbach_arr = goldbach_data->goldbach_arr;
  int64_t position = 0;

  int32_t num_process_error = EXIT_SUCCESS;

  // look for numbers while there are number to process
  while (true) {
    pthread_mutex_lock(&(goldbach_data->can_access_position)); {
      position = goldbach_data->last_processed_position;
      (goldbach_data->last_processed_position)++;
    } pthread_mutex_unlock(&(goldbach_data->can_access_position));

    // if out of bounds, quit
    if (position >= goldbach_get_arr_count(goldbach_arr)) {
      break;
    }

    num_process_error = num_golbach_process(goldbach_data, position);

    if (num_process_error != EXIT_SUCCESS) {
      return (void*)(int64_t)num_process_error;
    }
  }
  return (void*)(int64_t)num_process_error;
}

/**
 * Processes operations to determine goldbach sums
 * 
 */
int32_t num_golbach_process(goldbach_conjecture_t* goldbach_data,
const int64_t position) {
  int32_t num_process_error = EXIT_SUCCESS;

  int64_t current_num =
  goldbach_get_current_number(goldbach_data->goldbach_arr, position);

  bool positive = true;

  // if the number is negative, then make it positive
  if (current_num < 0) {
    current_num = -current_num;
    positive = false;
  }

  // if less than 6, then nothing to be done
  if (current_num < 6 && current_num != 4) {
    return num_process_error;
  }

  // process according to if the number is even or odd
  if (current_num % 2 == 0) {
    num_process_error = goldbach_even_process(goldbach_data,
    current_num, positive, position);
  } else {
    num_process_error = goldbach_odd_process(goldbach_data,
    current_num, positive, position);
  }

  return num_process_error;
}

/**
 * processes an odd number according to the goldbach conjecture
 * 
 */
int32_t goldbach_odd_process(goldbach_conjecture_t* goldbach_data,
const int64_t number, bool positive, const int64_t position) {
  const int32_t size = 3;
  int64_t third_number = 0;
  int32_t num_process_error = EXIT_SUCCESS;

  // allocate space for the sum
  int64_t* current_sum = malloc(size * sizeof(int64_t));

  // defensive for current_sum
  if (current_sum == NULL) {
    return error_sum_memory_allocation_failure;
  }

  // for each number less than half the given number
  for (int64_t current_number = 2; current_number < number/2;
  current_number = find_next_prime(current_number)) {
    // for each number less than the number minus the current number by half
    for (int64_t current_second_number = 2;
    current_second_number <= (number - current_number)/2;
    current_second_number = find_next_prime(current_second_number)) {
      /* find a third number (the number minus the current number and 
      the second current number)
      */
      third_number = number - (current_second_number + current_number);

      // check if the number if prime and valid
      if (isPrimeNum(third_number) &&
      third_number >= current_second_number &&
      current_second_number >= current_number) {
        // if so then add to the goldbach_arr
        if (!positive) {
          current_sum[0] = current_number;
          current_sum[1] = current_second_number;
          current_sum[2] = third_number;

          num_process_error =
          goldbach_add_sum(goldbach_data->goldbach_arr, current_sum, position);
        } else {
          goldbach_add_ghost_sum(goldbach_data->goldbach_arr, position);
        }
      }
    }
  }

  goldbach_finish_num_sums(goldbach_data->goldbach_arr, position);
  // free allocated space
  free(current_sum);

  return num_process_error;
}

/**
 * processes an even number according to the goldbach conjecture
 * 
 */
int32_t goldbach_even_process(goldbach_conjecture_t* goldbach_data,
const int64_t number, bool positive, const int64_t position) {
  int32_t num_process_error = EXIT_SUCCESS;

  const int32_t size = 2;
  int64_t other_number = 0;

  // allocate space for the sum
  int64_t* current_sum = malloc(size * sizeof(int64_t));

  // for each current number less than half the given number
  for (int64_t current_number = 2; current_number <= number/2;
  current_number = find_next_prime(current_number)) {
    // define a other_number (given number - current_number)
    other_number = number - current_number;

    // if both numbers are prime
    if (isPrimeNum(other_number)) {
      // add to goldbach_arr
      if (!positive) {
        current_sum[0] = current_number;
        current_sum[1] = other_number;

        num_process_error =
        goldbach_add_sum(goldbach_data->goldbach_arr, current_sum, position);
      } else {
        goldbach_add_ghost_sum(goldbach_data->goldbach_arr, position);
      }
    }
  }

  goldbach_finish_num_sums(goldbach_data->goldbach_arr, position);

  // free allocated memory space
  free(current_sum);

  return num_process_error;
}

/**
 * Finds the next prime number after the given number
 */
int64_t find_next_prime(const int64_t last_prime) {
  // if even start at next number
  int64_t number = last_prime + 1;

  // if odd then advance one more
  if (last_prime % 2 != 0) {
    number += 1;
  }

  // while number is not prime, check all odd numbers
  while (!isPrimeNum(number)) {
    number+= 2;
  }

  return number;
}

/**
 * checks if a given number is prime
 * based on the primality test algorithm in
 * https://en.wikipedia.org/wiki/Primality_test
 * 
 * Based on the principle where all prime numbers greater than 3
 * can be representad as 6k + 1
 */
bool isPrimeNum(const int64_t number) {
  int64_t comparator = 5;

  // check if number is two or three
  if (number == 2 || number == 3) {
    return true;
  }

  // check if number is 1, even or divisible by 3
  if (number % 2 == 0 || number %3 == 0 || number == 1) {
    return false;
  }

  // while not divisible by comparator number and within limits
  while (number % comparator != 0
  && number % (comparator + 2) != 0
  && comparator*comparator <= number) {
    // check all numbers every 6
    comparator += 6;
  }

  // number is prime if reached comparator is less than limit
  return comparator * comparator > number;
}

/**
 * Prints the results after processing the solutions
 */
void goldbach_print_sums(goldbach_conjecture_t* goldbach_conjecture) {
  int64_t size = 0, sum_amount = 0;
  int64_t* current_sum = NULL;

  // print totals
  printf("Total %" PRId64 " numbers %" PRId64 " sums\n\n",
  goldbach_get_arr_count(goldbach_conjecture->goldbach_arr),
  goldbach_arr_get_total_sums_amount(goldbach_conjecture->goldbach_arr));

  // for each number
  for (int64_t num = 0;
  num < goldbach_get_arr_count(goldbach_conjecture->goldbach_arr);
  num++) {

  int64_t current_num =
  goldbach_get_current_number(goldbach_conjecture->goldbach_arr, num);

    print_sums_amount(goldbach_conjecture->goldbach_arr, &sum_amount, current_num, num);

    if (sum_amount == 0) {
      continue;
    }

    // check if sums need to be printed
    if (current_num < 0) {
      printf(": ");
      // if so, for all sums for the given number
      print_sums_for_num(goldbach_conjecture->goldbach_arr, sum_amount, current_sum, &size, num);
    }
    printf("\n");
  }
}

/**
 * Prints the amount of sums found for a given number
 * 
 */
void print_sums_amount(goldbach_arr_t* goldbach_arr, int64_t* sum_amount,
const int64_t current_num, const int num) {
  if (sum_amount == NULL) {
    return;
  }

  // print sums amount
  *sum_amount = goldbach_get_sums_amount(goldbach_arr, num);

  printf("%" PRId64 ": ", current_num);

  // if none, print accordingly
  if (*sum_amount == 0) {
    printf("NA\n");
  } else {  // otherwise print sums amount
    printf("%" PRId64 " sums", *sum_amount);
  }
}

/**
 * Prints the actual sums found for a given number
 * 
 */
void print_sums_for_num(goldbach_arr_t* goldbach_arr,
const int64_t sum_amount, int64_t* current_sum,
int64_t* size, const int64_t num) {
  for (int64_t sum_in_num = 0;
  sum_in_num < sum_amount; sum_in_num++) {
    current_sum =
    goldbach_get_sum(goldbach_arr, size, num, sum_in_num);

    // for all numbers of the sum
    for (int64_t sum_element = 0;
    sum_element < *size; ++sum_element) {
      // print the results
      printf("%" PRId64 "", current_sum[sum_element]);
      if (sum_element != *size - 1) {
        printf(" + ");
      }
    }

    // free the space of the given sum
    free(current_sum);

    if (sum_in_num != sum_amount - 1) {
      printf(", ");
    }
  }
}


/**
 * Frees all alocated memory for goldbach_conjecture
 * 
 */
void goldbach_conjecture_destroy(goldbach_conjecture_t* goldbach_conjecture) {
  goldbach_arr_destroy(goldbach_conjecture->goldbach_arr);

  pthread_mutex_destroy(&goldbach_conjecture->can_access_position);

  free(goldbach_conjecture);
}