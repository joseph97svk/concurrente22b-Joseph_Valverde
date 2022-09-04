// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include "Goldbach_arr.h"

/**
 * @brief enum for error handling
 * 
 */
enum goldbach_conjecture_error {
    error_invalid_input_given = 150
};

/**
 * @brief reads the input to the console and adds them to the 
 * goldbach_arr for later processing
 * @details DOES NOT SOLVE anything, just reads.
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers read 
 * will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_read_numbers(goldbach_arr_t* goldbach_arr);

/**
 * @brief Processes and solves the numbers according to the 
 * Golbach conjecture
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers 
 * to be processed are store and where solutions will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_process_sums(goldbach_arr_t* goldbach_arr);

/**
 * @brief Prints the results after processing the solutions
 * @details DO NOT print before solving, undefined behaviour
 * 
 * @param goldbach_arr goldbach_arr from where sums are extracted
 */
void goldbach_print_sums(goldbach_arr_t* goldbach_arr);
