#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include "Goldbach_conjecture.h"

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
 * @brief Processess a given number according to the goldbach conjecture
 * 
 * @param goldbach_arr where the results will be stored
 * @param number number to be processed
 * @param position position within the goldbach_arr of the number
 * @return int32_t 
 */
int32_t goldbach_process_num (goldbach_arr_t* goldbach_arr, const int64_t number, const int64_t position);

/**
 * @brief processes an odd number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num
 * @param num @see goldbach_process_num
 * @param position @see goldbach_process_num
 * @return int32_t 
 */
int32_t goldbach_odd_process(goldbach_arr_t* goldbach_arr, const int64_t num, const int64_t position);

/**
 * @brief processes an even number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num
 * @param num @see goldbach_process_num
 * @param position @see goldbach_process_num
 * @return int32_t 
 */
int32_t goldbach_even_process(goldbach_arr_t* goldbach_arr, const int64_t num, const int64_t position);

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
 * 
 * @param number the number to be checked
 * @return true if the number is prime
 * @return false if the number is not prime
 */
bool isPrimeNum (const int64_t number);

/**
 * @brief reads the input to the console and adds them to the 
 * goldbach_arr for later processing
 * @details DOES NOT SOLVE anything, just reads.
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers read 
 * will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_read_numbers(goldbach_arr_t* goldbach_arr) {
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
            number_addition_error = goldbach_add_num(goldbach_arr, current_val_read);

            if (number_addition_error != EXIT_SUCCESS) {
                return number_addition_error;
            }
        } else {
            // if not valid, report so
            return error_invalid_input_given;
        }

        //ensure all values on input space to be 0 for next run
        for (int32_t character = 0; character < erase_space; character++) {
            input_read[character] = '0';
        }
    }

    return number_addition_error;
}

/**
 * @brief Processes and solves the numbers according to the 
 * Golbach conjecture
 * 
 * @param goldbach_arr pointer to goldbach_arr where numbers 
 * to be processed are store and where solutions will be stored
 * @return int32_t error-success state
 */
int32_t goldbach_process_sums(goldbach_arr_t* goldbach_arr) {
    int64_t size = goldbach_get_arr_count(goldbach_arr);
    int32_t num_process_error = EXIT_SUCCESS;

    int64_t current_num = 0;

    // for all elements/numbers in a goldbach_arr
    for (int64_t number = 0; number < size; number++) {

        current_num = goldbach_get_current_number(goldbach_arr, number);
        // process the given number
        num_process_error = goldbach_process_num(goldbach_arr, current_num, number);

        if (num_process_error != EXIT_SUCCESS) {
            return num_process_error;
        }
    }

    return num_process_error;
}

/**
 * @brief Processess a given number according to the goldbach conjecture
 * 
 * @param goldbach_arr where the results will be stored
 * @param number number to be processed
 * @param position position within the goldbach_arr of the number
 * @return int32_t 
 */
int32_t goldbach_process_num (goldbach_arr_t* goldbach_arr, const int64_t number, const int64_t position) {
    int64_t current_num = number;
    int32_t num_process_error = EXIT_SUCCESS;
    // if the number is negative, then make it positive
    if (current_num < 0) {
        current_num = -current_num;
    }

    // if less than 6, then nothing to be done
    if (current_num < 6) {
       return num_process_error;
    }

    // process according to if the number is even or odd
    if (current_num % 2 == 0) {
        num_process_error = goldbach_even_process(goldbach_arr, current_num, position);
    } else {
        num_process_error = goldbach_odd_process(goldbach_arr, current_num, position);
    }

    return num_process_error;
}

/**
 * @brief processes an odd number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num
 * @param num @see goldbach_process_num
 * @param position @see goldbach_process_num
 * @return int32_t 
 */
int32_t goldbach_odd_process(goldbach_arr_t* goldbach_arr, const int64_t number, const int64_t position) {
    const int32_t size = 3;
    int64_t third_number = 0;
    int32_t num_process_error = EXIT_SUCCESS;
    
    // allocate space for the sum
    int64_t* current_sum = malloc(size * sizeof(int64_t));

    // for each number less than half the given number
    for (int64_t current_number = 2; current_number < number/2; 
    current_number = find_next_prime(current_number)) {

        // for each number less than the number minus the current number by half
        for (int64_t current_second_number = 2; 
        current_second_number <= (number - current_number)/2;
        current_second_number = find_next_prime(current_second_number)) {

            // find a third number (the number minus the current number and the second current number)
            third_number = number - (current_second_number + current_number);
            
            // check if the number if prime and valid
            if (isPrimeNum(third_number) && third_number >= current_second_number &&
            current_second_number >= current_number) {
                // if so then add to the goldbach_arr
                current_sum[0] = current_number;
                current_sum[1] = current_second_number;
                current_sum[2] = third_number;

                num_process_error = goldbach_add_sum(goldbach_arr, current_sum, position);
            }
        }
    }

    // free allocated space
    free (current_sum);

    return num_process_error;
}

/**
 * @brief processes an even number according to the goldbach conjecture
 * 
 * @param goldbach_arr @see goldbach_process_num
 * @param num @see goldbach_process_num
 * @param position @see goldbach_process_num
 * @return int32_t 
 */
int32_t goldbach_even_process(goldbach_arr_t* goldbach_arr, const int64_t number, const int64_t position) {
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
            current_sum[0] = current_number;
            current_sum[1] = other_number;
            
            num_process_error = goldbach_add_sum(goldbach_arr, current_sum, position);
        }
    } 

    // free allocated memory space
    free (current_sum);

    return num_process_error;
}

/**
 * @brief Finds the next prime number after the given number
 * @details last_prime may or may not be prime,
 * will still, regardless, find the next prime number
 * 
 * @param last_prime number after which the next prime number is to be found
 * @return int64_t 
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
 * @brief checks if a given number is prime
 * 
 * @param number the number to be checked
 * @return true if the number is prime
 * @return false if the number is not prime
 */
bool isPrimeNum (const int64_t number) {
    int64_t comparator = 3, limit = number/2;

    // check if number is two
    if (number == 2) {
        return true;
    } 

    // check if number is even or is 1
    if (number % 2 == 0 || number == 1) {
        return false;
    }

    // while not divisible by comparator number and within limits
    while (number % comparator != 0  && comparator <= limit) {
        // check all odd numbers
        comparator += 2;
    }
    
    // number is prime if reached comparator is less than the number/2
    return comparator >= limit;
}

/**
 * @brief Prints the results after processing the solutions
 * @details DO NOT print before solving, undefined behaviour
 * 
 * @param goldbach_arr goldbach_arr from where sums are extracted
 */
void goldbach_print_sums(goldbach_arr_t* goldbach_arr) {
    int64_t size = 0, sum_amount = 0;
    int64_t* current_sum;

    // print totals
    printf("Total: %li numbers %li sums\n\n", goldbach_get_arr_count(goldbach_arr), 
    goldbach_arr_get_total_sums_amount(goldbach_arr));

    // for each number 
    for (int64_t num = 0; num < goldbach_get_arr_count(goldbach_arr); num++) {
        int64_t current_num = goldbach_get_current_number(goldbach_arr, num);

        // print sums amount
        sum_amount = goldbach_get_sums_amount(goldbach_arr, num);

        printf("%li: ", current_num);

        // if none, print accordingly
        if (sum_amount == 0) {
            printf("NA\n");
            continue;
        } else {  //otherwise print sums amount
            printf("%li sums", sum_amount);
        }
        
        // check if sums need to be printed
        if (current_num < 0) {
            printf(": ");
            // if so, for all sums for the given number
            for (int64_t sum_in_num = 0; sum_in_num < sum_amount; sum_in_num++) {
                current_sum = goldbach_get_sum(goldbach_arr, &size, num, sum_in_num);

                // for all numbers of the sum
                for (int64_t sum_element = 0; sum_element < size; ++sum_element) {
                    // print the results
                    printf("%" PRId64 "", current_sum[sum_element]);
                    if (sum_element != size -1) {
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
        printf("\n");
    }
}

/**
 * @brief checks if the given input is valid and gives the value
 * 
 * @param string to be checked
 * @param value where the value will be stored
 * @return true if value is valid
 * @return false if value is invalid
 */
bool num_validity_check(char string[64], int64_t* value) {
    errno = 0;

    char* end_ptr;

    // if the first char is not a number
    if (string[0] < 48 || string[0] > 57) {
        
        // if the first char is not num, and also not a minus
        if (string[0] != '-') {
            return false;
        // otherwise if is minus but is only that
        } else if (strlen(string) == 1) {
            return false;
        }
    }

    // begin checking at second char if it is num (first already checked)
    for (size_t character = 1; character < strlen(string) - 1; character++) {
        // if subsequent chars are not nums
        if (string[character] < 48 || string[character] > 57) {
            // then invalid
            return false;
        }
    }

    // change input to number
    int64_t number = strtol(string, &end_ptr, 10);

    // check if there was an overflow
    if (errno != 0) {
        return false;
    }

    *value = number;

    return true;
}

