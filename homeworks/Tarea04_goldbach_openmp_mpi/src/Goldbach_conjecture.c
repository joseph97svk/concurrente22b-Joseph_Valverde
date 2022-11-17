// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <omp.h>
#include <mpi.h>

#include "Goldbach_conjecture.h"
#include "goldbach_conjecture_structure.h"

#define CAST(type, unit)\
  (type)(unit)
  // reinterpret_cast<type>(unit)

#define MIN(a, b)\
  (a < b)? a : b

typedef int64_t unit_t;

int32_t add_number(goldbach_conjecture_t* goldbach_conjecture,
unit_t current_val_read);

/**
 * @brief checks if the given input is valid and gives the value
 * 
 * @param string to be checked
 * @param value where the value will be stored
 * @return true if value is valid
 * @return false if value is invalid
 */
bool num_validity_check(char string[64], unit_t* value);

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
 * @brief processess numbers with mpi processes
 * 
 * @param goldbach_conjecture 
 */
void mpi_process_nums(int32_t number_amount,
int64_t* numbers, int32_t thread_amount);

/**
 * @brief runs processing of goldbach conjecture numbers with
 * mpi processes with rank 0
 * @details this process acts as main process and coordinator
 * Will also be the one running for default cases and for 1 process amount
 * @param size amount of mpi processes
 * @param number_amount amount of numbers
 * @param numbers the numbers to be processed
 * @param thread_amount amount of threads expected for execution
 */
void mpi_rank_0(int size, int number_amount,
int64_t* numbers, int32_t thread_amount);

/**
 * @brief runs processing of goldbach conjecture numbers
 * for all processes with rank other than 0
 * 
 * @param rank rank or id of process
 * @param size amount of mpi processes for program
 * @param thread_amount amount of threads expected for execution
 */
void mpi_rank_other(int rank, int size, int32_t thread_amount);

/**
 * @brief Runs the processing of goldbach numbers for non 0 rank processes
 * 
 * @param data struct with necessary data for processing
 * @param number_amount amount of numbers
 * @param numbers number to be processed
 * @param thread_amount amount of threads expected for execution
 */
void mpi_goldbach_process(goldbach_data_t* data,
    int number_amount, int64_t* numbers, int32_t thread_amount);

/**
 * @brief processes numbers with threads
 * 
 * @param goldbach_conjecture 
 * @return int32_t 
 */
int32_t run_threads(goldbach_data_t* goldbach_conjecture,
int32_t thread_amount, int32_t number_amount);

/**
 * @brief Processes operations to determine goldbach sums
 * 
 * @param goldbach_data 
 * @param position 
 * @return int32_t 
 */
int32_t num_golbach_process(goldbach_data_t* goldbach_data,
const int64_t position);

/**
 * @brief processes an odd number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num
 * @param num @see goldbach_process_num
 * @param position @see goldbach_process_num
 * @return int32_t 
 */

int32_t goldbach_odd_process(goldbach_data_t* goldbach_data,
const unit_t num, bool positive, const int64_t position);

/**
 * @brief processes an even number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num()
 * @param num @see goldbach_process_num()
 * @param position @see goldbach_process_num()
 * @return int32_t 
 */

int32_t goldbach_even_process(goldbach_data_t* goldbach_data,
const unit_t num, bool positive, const int64_t position);

/**
 * @brief finds all prime numbers needed to process all numbers
 * 
 * @param goldbach_conjecture where all needed data is found and where it will be stored
 * @param initPos first position to process
 * @param finalPos last position to process
 */
void prime_search_atkins_sieve(goldbach_data_t* goldbach_conjecture,
int64_t initPos, int64_t finalPos);

/**
 * @brief Finds the next prime number after the given number
 * @details last_prime may or may not be prime,
 * will still, regardless, find the next prime number
 * 
 * @param last_prime number after which the next prime number is to be found
 * @return int64_t 
 */

unit_t find_next_prime(const unit_t last_prime,
goldbach_data_t* goldbach_conjecture);

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

bool isPrimeNum(const unit_t number, goldbach_data_t* goldbach_data);

/**
 * @brief prints sums for mpi functions
 * 
 * @param goldbach_conjecture 
 */
void mpi_print_sums(goldbach_data_t* goldbach_conjecture);

/**
 * @brief Prints the amount of sums found for a given number
 * 
 * @param goldbach_arr
 * @param sum_amount amount of sums found for the number
 * @param current_num the actual number
 * @param num position of the number within the array
 */
void print_sums_amount(goldbach_arr_t* goldbach_arr, int64_t* sum_amount,
const unit_t current_num, const int num);

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
const int64_t sum_amount, unit_t* current_sum,
int64_t* size, const int64_t num);


goldbach_data_t* goldbach_data_init() {
  goldbach_data_t* data = calloc(1, sizeof(goldbach_data_t));

  if (data == NULL) {
    return NULL;
  }

  data->goldbach_arr = goldbach_arr_create();

  if (data->goldbach_arr == NULL) {
    free(data);
    return NULL;
  }

  return data;
}

/**
 * @brief returns a set up goldbach_arr based on the
 * arguments/paramenters
 * @param argc amount of arguments
 * @param argv arguments
 * @return golbach_conjecture_t* 
 */
goldbach_conjecture_t* goldbach_set_up(int argc, char** argv) {
  if (argc > 2) {
    return NULL;
  }

  goldbach_conjecture_t* goldbach_conjecture = CAST(goldbach_conjecture_t*,
  calloc(1, sizeof(goldbach_conjecture_t)));

  if (goldbach_conjecture == NULL) {
    return NULL;
  }

  // goldbach_conjecture->goldbach_arr = goldbach_arr;
  goldbach_conjecture->thread_amount = 0;
  goldbach_conjecture->thread_amount = 0;
  goldbach_conjecture->max_value = 0;
  goldbach_conjecture->number_capacity = 10;
  goldbach_conjecture->number_count = 0;
  goldbach_conjecture->numbers =
  calloc(goldbach_conjecture->number_capacity, sizeof(int64_t));

  if (goldbach_conjecture->numbers == NULL) {
    free(goldbach_conjecture);
    return NULL;
  }

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
  unit_t current_val_read = 0;

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
      add_number(goldbach_conjecture, current_val_read);

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

int32_t add_number(goldbach_conjecture_t* goldbach_conjecture,
unit_t current_val_read) {
  int32_t number_addition_error = EXIT_SUCCESS;

  if (goldbach_conjecture->number_capacity ==
  goldbach_conjecture->number_count) {
    goldbach_conjecture->number_capacity *= 10;
    int64_t* buffer = realloc(goldbach_conjecture->numbers,
    goldbach_conjecture->number_capacity * sizeof(int64_t));

    if (buffer == NULL) {
      return EXIT_FAILURE;
    } else {
      goldbach_conjecture->numbers = buffer;
    }
  }

  goldbach_conjecture->numbers[goldbach_conjecture->number_count] =
  current_val_read;

  goldbach_conjecture->number_count += 1;

  return number_addition_error;
}

/**
 * checks if the given input is valid and gives the value
 * 
 */
bool num_validity_check(char string[64], unit_t* value) {
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
  unit_t number = strtol(string, &end_ptr, 10); //NOLINT

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

  int32_t number_amount = goldbach_conjecture->number_count,
  thread_amount = goldbach_conjecture->thread_amount;
  int64_t* numbers = goldbach_conjecture->numbers;

  mpi_process_nums(number_amount, numbers, thread_amount);

  return num_process_error;
}

void mpi_process_nums(int32_t number_amount,
int64_t* numbers, int32_t thread_amount) {
  int rank = 0, size = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size == 1) {
    // process normally
    goldbach_data_t* data = goldbach_data_init();
    for (int num = 0; num < number_amount; ++num) {
      goldbach_add_num(data->goldbach_arr, numbers[num]);
    }

    data->max_value =
    goldbach_arr_get_max_value(data->goldbach_arr);

    if (thread_amount == 0) {
      thread_amount = sysconf(_SC_NPROCESSORS_ONLN);
    }

    // process data
    run_threads(data, thread_amount, number_amount);

    // send total amount of sums
    int sums_amount =
    goldbach_arr_get_total_sums_amount(data->goldbach_arr);

    // print totals
    printf("Total %i numbers %i sums\n\n",
    number_amount, sums_amount);

    mpi_print_sums(data);

    goldbach_arr_destroy(data->goldbach_arr);
    free(data->prime_number_list);
    free(data);

    return;
  }

  mpi_rank_0(size, number_amount, numbers, thread_amount);
}

int get_start_pos(const int i, const int d, const int w) {
  int base = d/w;
  int minimum = MIN(i, d%w);
  int result = ((i * (base)) + minimum);
  return result;
}

void mpi_rank_0(int size, int number_amount,
    int64_t* numbers, int32_t thread_amount) {
  int32_t* processes = calloc(size -1, sizeof(int32_t));
  int32_t rank_to_send = -1;

  // printf("size: %i\n", size);
  for (int process = 0; process < size - 1; ++process) {
    // printf("process: %i\n", process);
    int start_pos = get_start_pos(process, number_amount, size - 1);
    int stop_pos = get_start_pos(process + 1, number_amount, size - 1);
    int block_size = stop_pos - start_pos;

    // printf(">receving rank\n");
    // receive ready
    MPI_Recv(&rank_to_send, 1,
    MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    processes[process] = rank_to_send;

    // printf(">sending thread amount\n");
    // send thread_amount
    MPI_Send(&thread_amount, /* amount */1, MPI_INT,
        rank_to_send, 0, MPI_COMM_WORLD);

    // printf(">sending number amount: %i\n", block_size);
    // send number amount
    MPI_Send(&block_size, /* amount */1, MPI_INT,
        rank_to_send, 0, MPI_COMM_WORLD);

    // printf(">sending numbers: %i\n", start_pos);
    // send numbers
    MPI_Send((void*) &numbers[start_pos], /* amount */block_size,
        MPI_LONG_LONG, rank_to_send, 0, MPI_COMM_WORLD);

    // printf(">finished providing\n");
  }
  int total_sums = 0;

  // printf(">receving totals\n");
  // receive totals
  for (int position = 1; position < size; ++position) {
    int local_sum = -1;
    MPI_Recv(&local_sum, 1, MPI_INT, position,
    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    total_sums += local_sum;
  }

  // print totals
  printf("Total %i numbers %i sums\n\n",
  number_amount, total_sums);

  for (int position = 0; position < size - 1; ++position) {
    // send order to print processes[position]
    MPI_Send(&position, /* amount */1, MPI_INT,
    processes[position], 0, MPI_COMM_WORLD);
  }

  free(processes);
}

void goldbach_mpi_process(int rank) {
  goldbach_data_t* data = goldbach_data_init();

  int32_t number_amount = -1, thread_amount = 0;

  int64_t* numbers = NULL;

  // printf("sending ready rank\n");
  // send ready
  MPI_Send(&rank, 1, MPI_INT, /*destination*/ 0, 0, MPI_COMM_WORLD);

  // printf("receving thread amount\n");
  // receive thread amount
  MPI_Recv(&thread_amount, 1,
  MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // if thread amount = 0, then use system amount
  if (thread_amount == 0) {
    thread_amount = sysconf(_SC_NPROCESSORS_ONLN);
  }

  // printf("receving number amount\n");
  // receive number amount
  MPI_Recv(&number_amount, 1,
  MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  mpi_goldbach_process(data, number_amount, numbers, thread_amount);

  goldbach_arr_destroy(data->goldbach_arr);
  free(data->prime_number_list);
  free(data);
}

void mpi_goldbach_process(goldbach_data_t* data,
    int number_amount, int64_t* numbers, int32_t thread_amount) {
  numbers = calloc(number_amount, sizeof(int64_t));

  // printf("receving numbers: %i\n", number_amount);
  // receive numbers
  MPI_Recv(numbers, number_amount,
  MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // printf("adding numbers\n");
  // add numbers
  for (int num = 0; num < number_amount; ++num) {
    // printf("num: %i: %li\n", num, numbers[num]);
    goldbach_add_num(data->goldbach_arr, numbers[num]);
    // printf("num added\n");
  }

  free(numbers);

  // printf("done adding numbers\n");

  data->max_value =
  goldbach_arr_get_max_value(data->goldbach_arr);

  // fprintf(stderr, "processing data\n");
  // process data
  run_threads(data, thread_amount, number_amount);

  // printf("data processed\n");
  // send total amount of sums
  int sums_amount =
  goldbach_arr_get_total_sums_amount(data->goldbach_arr);

  // printf("sending sum amount: %i\n", sums_amount);
  MPI_Send(&sums_amount, 1, MPI_INT, /*destination*/ 0, 0, MPI_COMM_WORLD);

  // printf("receiving order to print\n");
  // wait for order to print
  int order = -1;
  MPI_Recv(&order, 1,
  MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // print
  mpi_print_sums(data);
}

int32_t run_threads(goldbach_data_t* goldbach_conjecture,
int32_t thread_amount, int32_t number_amount) {
  int32_t num_process_error = EXIT_SUCCESS;

  if (number_amount < thread_amount) {
    thread_amount = number_amount;
  }

  if (thread_amount == 0) {
    return EXIT_SUCCESS;
  }

  #pragma omp parallel num_threads(thread_amount) \
    default(none) shared(goldbach_conjecture, num_process_error)
  {  // NOLINT
    int32_t local_error = EXIT_SUCCESS;

    // for all elements/numbers in a goldbach_arr
    #pragma omp for
    for (int64_t number = 0;
    number < goldbach_get_arr_count(goldbach_conjecture->goldbach_arr);
    number++) {
      local_error = num_golbach_process(goldbach_conjecture, number);

      if (local_error != EXIT_SUCCESS) {
        #pragma omp critical
        num_process_error = local_error;
      }
    }
  }

  return num_process_error;
}

/**
 * Processes operations to determine goldbach sums
 * 
 */
int32_t num_golbach_process(goldbach_data_t* goldbach_data,
const int64_t position) {
  int32_t num_process_error = EXIT_SUCCESS;

  unit_t current_num =
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
int32_t goldbach_odd_process(goldbach_data_t* goldbach_data,
const unit_t number, bool positive, const int64_t position) {
  const int32_t size = 3;
  unit_t third_number = 0;
  int32_t num_process_error = EXIT_SUCCESS;

  // allocate space for the sum
  unit_t* current_sum = CAST(unit_t*, malloc(size * sizeof(unit_t)));

  // defensive for current_sum
  if (current_sum == NULL) {
    return error_sum_memory_allocation_failure;
  }

  // for each number less than half the given number
  for (unit_t current_number = 2; current_number < number/2;
  current_number = find_next_prime(current_number, goldbach_data)) {
    // for each number less than the number minus the current number by half
    for (unit_t current_second_number = 2;
    current_second_number <= (number - current_number)/2;
    current_second_number =
    find_next_prime(current_second_number, goldbach_data)) {
      /* find a third number (the number minus the current number and 
      the second current number)
      */
      third_number = number - (current_second_number + current_number);

      // check if the number if prime and valid
      if (isPrimeNum(third_number, goldbach_data) &&
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
int32_t goldbach_even_process(goldbach_data_t* goldbach_data,
const unit_t number, bool positive, const int64_t position) {
  int32_t num_process_error = EXIT_SUCCESS;

  const int32_t size = 2;
  unit_t other_number = 0;

  // allocate space for the sum
  unit_t* current_sum = CAST(unit_t*, malloc(size * sizeof(unit_t)));

  // for each current number less than half the given number
  for (unit_t current_number = 2; current_number <= number/2;
  current_number = find_next_prime(current_number, goldbach_data)) {
    // define a other_number (given number - current_number)
    other_number = number - current_number;

    // if both numbers are prime
    if (isPrimeNum(other_number, goldbach_data)) {
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
unit_t find_next_prime(const unit_t last_prime,
goldbach_data_t* goldbach_conjecture) {
  // if even start at next number
  unit_t number = last_prime + 1;

  // if odd then advance one more
  if (last_prime % 2 != 0) {
    number += 1;
  }

  // while number is not prime, check all odd numbers
  while (!isPrimeNum(number, goldbach_conjecture)) {
    number+= 2;
  }

  return number;
}

// finds all prime numbers needed to process all numbers
void prime_search_atkins_sieve(goldbach_data_t* goldbach_conjecture,
int64_t initPos, int64_t finalPos) {
  int64_t limit = finalPos;
  goldbach_conjecture->prime_number_list =
  calloc(limit + 1, sizeof(bool));

  bool* sieve = goldbach_conjecture->prime_number_list;

  if (sieve == NULL) {
    goldbach_conjecture->prime_number_list_capacity = 0;
    return;
  }

  goldbach_conjecture->prime_number_list_capacity = limit;
  if (limit > 2) {
    goldbach_conjecture->prime_number_list[2] = true;
  }

  if (limit > 3) {
    sieve[3] = true;
  }

  for (int x = initPos; x * x <= limit; x++) {
    for (int y = initPos; y * y <= limit; y++) {
      int n = (4 * x * x) + (y * y);
      if (n <= limit &&
      (n % 12 == 1 || n % 12 == 5)) {
        sieve[n] ^= true;
      }

      n = (3 * x * x) + (y * y);
      if (n <= limit && n % 12 == 7) {
        sieve[n] ^= true;
      }

      n = (3 * x * x) - (y * y);
      if (x > y && n <= limit &&
      n % 12 == 11) {
        sieve[n] ^= true;
      }
    }
  }

  for (int num = 5; num * num <= limit; num++) {
    if (sieve[num]) {
      for (int other = num * num;
      other <= limit;
      other += (num * num)) {
        sieve[other] = false;
      }
    }
  }
}

/**
 * checks if a given number is prime
 * based on the primality test algorithm in
 * https://en.wikipedia.org/wiki/Primality_test
 * 
 * Based on the principle where all prime numbers greater than 3
 * can be representad as 6k + 1
 */
bool isPrimeNum(const unit_t number, goldbach_data_t* goldbach_data) {
  unit_t comparator = 5;
  if (number < goldbach_data->prime_number_list_capacity) {
    return goldbach_data->prime_number_list[number];
  }

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
  unit_t* current_sum = NULL;

  // print totals
  printf("Total %" PRId64 " numbers %" PRId64 " sums\n\n",
  goldbach_get_arr_count(goldbach_conjecture->goldbach_arr),
  goldbach_arr_get_total_sums_amount(goldbach_conjecture->goldbach_arr));

  // for each number
  for (int64_t num = 0;
  num < goldbach_get_arr_count(goldbach_conjecture->goldbach_arr);
  num++) {
  unit_t current_num =
  goldbach_get_current_number(goldbach_conjecture->goldbach_arr, num);

    print_sums_amount(goldbach_conjecture->goldbach_arr,
    &sum_amount, current_num, num);

    if (sum_amount == 0) {
      continue;
    }

    // check if sums need to be printed
    if (current_num < 0) {
      printf(": ");
      // if so, for all sums for the given number
      print_sums_for_num(goldbach_conjecture->goldbach_arr,
      sum_amount, current_sum, &size, num);
    }
    printf("\n");
  }
}

void mpi_print_sums(goldbach_data_t* goldbach_conjecture) {
  int64_t size = 0, sum_amount = 0;
  unit_t* current_sum = NULL;

  // for each number
  for (int64_t num = 0;
  num < goldbach_get_arr_count(goldbach_conjecture->goldbach_arr);
  num++) {
  unit_t current_num =
  goldbach_get_current_number(goldbach_conjecture->goldbach_arr, num);

    print_sums_amount(goldbach_conjecture->goldbach_arr,
    &sum_amount, current_num, num);

    if (sum_amount == 0) {
      continue;
    }

    // check if sums need to be printed
    if (current_num < 0) {
      printf(": ");
      // if so, for all sums for the given number
      print_sums_for_num(goldbach_conjecture->goldbach_arr,
      sum_amount, current_sum, &size, num);
    }
    printf("\n");
  }
}

/**
 * Prints the amount of sums found for a given number
 * 
 */
void print_sums_amount(goldbach_arr_t* goldbach_arr, int64_t* sum_amount,
const unit_t current_num, const int num) {
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
const int64_t sum_amount, unit_t* current_sum,
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
 */
void goldbach_conjecture_destroy(goldbach_conjecture_t* goldbach_conjecture) {
  free(goldbach_conjecture->numbers);
  free(goldbach_conjecture);
}
