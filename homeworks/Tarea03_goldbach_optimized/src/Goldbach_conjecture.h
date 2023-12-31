#pragma once

// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include "Goldbach_arr.h"
#include <pthread.h>
#include <unistd.h>

/**
 * @brief enum for error handling
 * 
 */
enum goldbach_conjecture_error {
    error_invalid_input_given = 150,
    error_sum_memory_allocation_failure,
};

struct goldbach_conjecture;

typedef struct goldbach_conjecture goldbach_conjecture_t;

/**
 * @brief returns a set up goldbach_arr based on the
 * arguments/paramenters
 * @param argc amount of arguments
 * @param argv arguments
 * @return goldbach_arr_t* 
 */
goldbach_conjecture_t* goldbach_set_up(int argc, char* argv[]);

/**
 * @brief reads the input to the console and adds them to the 
 * goldbach_arr for later processing
 * @details DOES NOT SOLVE anything, just reads.
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers read 
 * will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_read_numbers(goldbach_conjecture_t* goldbach_conjecture);

/**
 * @brief Processes and solves the numbers according to the 
 * Golbach conjecture
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers 
 * to be processed are store and where solutions will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_process_sums(goldbach_conjecture_t* goldbach_conjecture);

/**
 * @brief Prints the results after processing the solutions
 * @details DO NOT print before solving, undefined behaviour
 * 
 * @param goldbach_arr goldbach_arr from where sums are extracted
 */
void goldbach_print_sums(goldbach_conjecture_t* goldbach_conjecture);

/**
 * @brief Frees all alocated memory for goldbach_conjecture
 * 
 * @param goldbach_arr 
 */
void goldbach_conjecture_destroy(goldbach_conjecture_t* goldbach_conjecture);
