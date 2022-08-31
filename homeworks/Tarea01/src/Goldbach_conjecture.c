#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include "Goldbach_conjecture.h"

bool num_validity_check(char string[64], int64_t* value);

int32_t goldbach_even_process(goldbach_arr_t* goldbach_arr, const int64_t num, const int64_t position);

int32_t goldbach_odd_process(goldbach_arr_t* goldbach_arr, const int64_t num, const int64_t position);

int64_t find_next_prime(const int64_t last_prime);

bool isPrimeNum (const int64_t number);

int32_t goldbach_read_numbers(goldbach_arr_t* goldbach_arr) {
    // space to read a value up to 64 digits
    char input_read[64];
    int64_t current_val_read = 0;


    // while numbers are being read 
    while (scanf("%63s[^\n]", input_read)) {
        if (input_read[0] == 'n') {
            return EXIT_SUCCESS;
        }

        int erase_space = strlen(input_read);        

        // check if input is valid
        if (num_validity_check(input_read, &current_val_read)) {
            // if valid, add them 
            goldbach_add_num(goldbach_arr, current_val_read);
        } else {
            // if not valid, report so
            return EXIT_FAILURE;
        }

        //ensure all values on input space to be 0 for next run
        for (int character = 0; character < erase_space; character++) {
            input_read[character] = '0';
        }
    }

    return EXIT_SUCCESS;
}

int32_t goldbach_process_sums(goldbach_arr_t* goldbach_arr) {
    int64_t size = goldbach_get_arr_length(goldbach_arr);

    int64_t current_num = 0;

    for (int number = 0; number < size; number++) {
        current_num = goldbach_get_current_number(goldbach_arr, number);

        if (current_num < 0) {
            current_num = -current_num;
        }

        if (current_num < 6) {
            return EXIT_SUCCESS;
        }

        if (current_num % 2 == 0) {
            goldbach_even_process(goldbach_arr, current_num, number);
        } else {
            goldbach_odd_process(goldbach_arr, current_num, number);
        }
    }

    return EXIT_SUCCESS;
}

int32_t goldbach_odd_process(goldbach_arr_t* goldbach_arr, const int64_t number, const int64_t position) {
    const int32_t size = 3;

    int64_t* current_sum = malloc(size * sizeof(int64_t));

    

    free (current_sum);

    return EXIT_SUCCESS;
}

int32_t goldbach_even_process(goldbach_arr_t* goldbach_arr, const int64_t number, const int64_t position) {
    const int32_t size = 2;
    int64_t other_number = 0;

    int64_t* current_sum = malloc(size * sizeof(int64_t));

    for (int64_t current_number = 1; current_number <= number/2; 
    current_number = find_next_prime(current_number)) {
        other_number = number - current_number;
        // if both numbers are prime
        if (isPrimeNum(other_number)) {
            current_sum[0] = current_number;
            current_sum[1] = other_number;
            goldbach_add_sum(goldbach_arr, current_sum, position);
        }
    } 

    free (current_sum);

    return EXIT_SUCCESS;
}

int64_t find_next_prime(const int64_t last_prime) {
    bool isPrime = false;
    int64_t number = last_prime + 1;

    while (!isPrime) {
        isPrime = isPrimeNum(number);
        if (!isPrime) {
            number+= 1;
        }
    }

    return number;
}

bool isPrimeNum (const int64_t number) {
    int64_t comparator = 3;

    if (number == 2) {
        return true;
    } 

    if (number % 2 == 0) {
        return false;
    }

    while (number % comparator != 0  && comparator <= number/2) {
        comparator += 2;
    }
        
    return comparator >= number/2;
}

void goldbach_print_sums(goldbach_arr_t* goldbach_arr) {
    int64_t size = 0, sum_amount = 0;
    int64_t* current_sum;


    for (int64_t num = 0; num < goldbach_get_arr_length(goldbach_arr); num++) {
        int64_t current_num = goldbach_get_current_number(goldbach_arr, num);

        printf("%li: ", current_num);

        sum_amount = goldbach_get_sums_amount(goldbach_arr, num);

        for (int64_t sum_in_num = 0; sum_in_num < sum_amount; sum_in_num++) {
            
            if (current_num < 0) {
                current_sum = goldbach_get_sum(goldbach_arr, &size, num, sum_in_num);

                for (int64_t sum_element = 0; sum_element < size; ++sum_element) {
                    printf("%" PRId64 " ", current_sum[sum_element]);
                }

                free(current_sum);
            }
        }
        printf("\n");
    }
}

bool num_validity_check(char string[64], int64_t* value) {
    errno = 0;

    char* end_ptr;

    if (string[0] < 48 || string[0] > 57) {

        if (string[0] != '-') {
            return false;
        } else if (strlen(string) == 1) {
            return false;
        }
    }

    for (size_t character = 1; character < strlen(string); character++) {
        if (string[character] < 48 || string[character] > 57) {
            return false;
        }
    }

    int64_t number = strtol(string, &end_ptr, 10);

    if (errno != 0) {
        return false;
    }

    *value = number;

    return true;
}

