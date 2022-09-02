#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Goldbach_conjecture.h"

int main(void) {
    // declare goldbach_arr
    goldbach_arr_t* goldbach_arr = goldbach_arr_create();

    if (goldbach_arr == NULL) {
        return EXIT_FAILURE;
    }
       
    // read values into it
    if (goldbach_read_numbers(goldbach_arr) == EXIT_SUCCESS) {

        // process the sums for read numbers
        goldbach_process_sums(goldbach_arr);

        // print the results
        goldbach_print_sums(goldbach_arr);
    } else {
        printf("Invalid value read\n");
    }

    goldbach_arr_destroy(goldbach_arr);
    
    return EXIT_SUCCESS;
}


