// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Goldbach_conjecture.h"
/**
 * @brief processes values in goldbach_arr
 * @details calls function from goldbach_conjecture.h
 * For error handling
 * @param goldbach_arr where values are stored and results will be stored
 * @return int 
 */
int process_goldbach_nums(goldbach_arr_t* goldbach_arr);

int main(void) {
    int32_t goldbach_error = EXIT_SUCCESS;

    // declare goldbach_arr
    goldbach_arr_t* goldbach_arr = goldbach_arr_create();

    if (goldbach_arr == NULL) {
        fprintf(stderr, "error: Goldbach_arr memory could not be allocated\n");
        return EXIT_FAILURE;
    }

    // read values into it
    goldbach_error = goldbach_read_numbers(goldbach_arr);

    // check if reading numbers was successful
    switch (goldbach_error) {
        // if successfull
        case EXIT_SUCCESS:
            // process the sums for read numbers
            goldbach_error = process_goldbach_nums(goldbach_arr);
            break;
        // otherwise:
        case error_invalid_input_given:
            fprintf(stderr, "error: Invalid value read\n");
            break;
        case error_goldbach_arr_elements_memory_allocation_failure:
            fprintf(stderr, "error: Memory could not be allocated to add number\n");
            break;
    }

    // if processing values was successful
    if (goldbach_error == EXIT_SUCCESS) {
        // print the results
        goldbach_print_sums(goldbach_arr);
    }

    if (goldbach_arr !=  NULL) {
        goldbach_arr_destroy(goldbach_arr);
    }

    return goldbach_error;
}


int process_goldbach_nums(goldbach_arr_t* goldbach_arr) {
    // process the values in the goldbach_arr
    int32_t goldbach_num_process_error =
    goldbach_process_sums(goldbach_arr);

    switch (goldbach_num_process_error) {
        // if successful
        case EXIT_SUCCESS:
            // do nothing, main will print the results
            break;
        // otherwise:
        case error_adding_sum_current_sum_memory_allocation_failure:
            fprintf(stderr,
            "error: Memory allocation issue when resizing goldbach_arr\n");
            break;
        case error_adding_sum_failure_invalid_position_or_given_sum:
            fprintf(stderr,
            "error: Indexing issue when adding sum to goldbach_arr\n");
            break;
        case error_adding_sum_provided_sum_invalid_size:
            fprintf(stderr, "error: Sum provided of invalid size\n");
            break;
        case error_adding_sum_sums_memory_allocation_failure:
            fprintf(stderr,
            "error: Memory allocation issue when adding sum to goldbach_arr\n");
            break;
        default:
            break;
        }

    return goldbach_num_process_error;
}
