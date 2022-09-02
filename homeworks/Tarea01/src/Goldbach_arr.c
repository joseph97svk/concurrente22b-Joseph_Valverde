#include "Goldbach_arr.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct goldbach_element {
    int64_t number;
    int64_t sum_amount;
    int64_t** sums;
} goldbach_element_t;

struct goldbach_arr {
    int64_t capacity;   // amount of space available
    int64_t count; // amount of contents 
    goldbach_element_t* elements;
};

int32_t goldbach_element_init(goldbach_element_t* element);

void delete_goldbach_element(goldbach_element_t* element);

goldbach_arr_t* goldbach_arr_create() {
    // allocate
    goldbach_arr_t* new_goldbach_arr = malloc(sizeof(goldbach_arr_t));

    if (new_goldbach_arr != NULL) {
        // initialize
        new_goldbach_arr -> count = 0;
        new_goldbach_arr -> capacity = 10;
        new_goldbach_arr -> elements = calloc(new_goldbach_arr -> capacity, sizeof(goldbach_element_t));

        /* if memory for elements could not be allocated, the goldbach arr creation
        * was not succesful, then free allocated space and return null
        */
        if (new_goldbach_arr -> elements == NULL) {
            free(new_goldbach_arr);
            new_goldbach_arr = NULL;
        }
    }
    
    return new_goldbach_arr;
}

int32_t goldbach_element_init(goldbach_element_t* element) {

    element -> number = 0;
    element -> sum_amount = 0;
    element -> sums = malloc(0);

    if (element -> sums == NULL) {
        fprintf(stderr, "Error: memory could not be allocated");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void delete_goldbach_element(goldbach_element_t* element){ 
    free (element -> sums);
}

int32_t goldbach_add_num(goldbach_arr_t* arr, const int64_t num) {
    if (arr -> elements == NULL) {
       arr = goldbach_arr_create();
    } else if (arr -> capacity == arr -> count) {
        // increase the count
        arr -> capacity = arr -> capacity + 10;

        // reallocate memory according to new capacity
        arr -> elements = realloc(arr -> elements, 
        ((arr -> capacity)) * sizeof (goldbach_element_t));
    }

    if (arr -> elements == NULL) {
        goldbach_arr_destroy(arr);
        return EXIT_FAILURE;
    }

    goldbach_element_init(&(arr -> elements)[arr -> count]);

    // set the number
    (arr -> elements)[arr -> count].number = num;

    // increase the count of the array
    arr -> count = (arr -> count) + 1;

    return EXIT_SUCCESS;
}

int32_t goldbach_add_sum(goldbach_arr_t* arr, const int64_t* const sum, const int64_t position) {   

    if (position >= arr -> count|| sum == NULL) {
        return EXIT_FAILURE;
    }

    if (arr -> elements[position].sums == NULL) {
        arr -> elements[position].sums = malloc(sizeof(int64_t**));
    }

    if (arr -> elements[position].sums == NULL) {
        return EXIT_FAILURE;
    }

    // get the position for the operations
    int64_t sum_position = arr -> elements[position].sum_amount;
    //printf("<<<<<%li>>>>\n", (arr -> length -> integer));

    // increase sum amount
    arr -> elements[position].sum_amount = (arr -> elements[position].sum_amount) + 1;

    // increase the amount of space in the array of pointers
    int64_t** sums_buffer = realloc(arr -> elements[position].sums, 
    (arr -> elements[position].sum_amount) * sizeof(int64_t**));

    //printf(">>>>%li<<<<<\n", arr -> elements[position] -> sum_amount);

    if (sums_buffer == NULL) {
        return EXIT_FAILURE;
    }

    // assign new address if succesful
    arr -> elements[position].sums = sums_buffer;

    // buffer the sum amount
    int64_t sum_element_amount = 3;

    if (arr -> elements[position].number % 2 == 0) {
        sum_element_amount = 2;
    }
    
    // allocate the space in the given pointer
    arr -> elements[position].sums[sum_position] = 
    malloc(sum_element_amount * sizeof(int64_t));

    const int64_t* pointer_check = sum;
    //printf("<%li, %li>\n(", arr -> elements[position] -> number, position);
    for (int32_t sum_element = 0; sum_element < sum_element_amount; sum_element++) {
        if (pointer_check == NULL) { // NOLINT
            return EXIT_FAILURE;
        }
        //printf("<%li, %i>", sum_position, sum_element);
        arr -> elements[position].sums[sum_position][sum_element] = sum[sum_element];
        pointer_check += (sizeof(int64_t*));
        
    }
    //printf(")\n");
    return EXIT_SUCCESS;
}

int64_t goldbach_get_arr_length(goldbach_arr_t* arr) {
    return arr -> count;
}

int64_t goldbach_get_current_number(goldbach_arr_t* arr, const int64_t position) {
    return arr -> elements[position].number;
}

int64_t goldbach_get_sums_amount(goldbach_arr_t* arr, const int64_t position) {
    return arr -> elements[position].sum_amount;
}

/// memory must be freed 
int64_t* goldbach_get_sum(const goldbach_arr_t* const arr, int64_t* size, const int64_t num_position, const int64_t sum_position) {
    // if out of bounds, then return null pointer
    if (num_position > arr -> count) {
        return NULL;
    }

    // if out of bounds, then return null pointer. After previous check to prevent undefined behaviour
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

    if (sum == NULL) {
        return NULL;
    }
    //printf(">>%li\n", arr -> elements[num_position] -> sum_amount);
    // copy the sum to the allocated array to be returned
    for (int64_t sum_num = 0; sum_num < *size; sum_num++) {
        //fprintf(stderr, "[%li, %li]", sum_position, sum_num);
        sum[sum_num] = arr -> elements[num_position].sums[sum_position][sum_num];
    }
    
    return sum;
}

// goldbach_arr_destroy
void goldbach_arr_destroy(goldbach_arr_t* arr) {
    for (int64_t element = 0; element < arr -> capacity; element++) {
        for (int64_t sum = 0; sum < arr -> elements[element].sum_amount; sum++) {
            if (arr -> elements[element].sums[sum] != NULL) {
                free(arr -> elements[element].sums[sum]);
            }
        }

        if (arr -> elements[element].sums != NULL) {
            fprintf("[%lu]\n", arr -> elements[element].sums);
            free(arr -> elements[element].sums);
        }
    }

    if (arr -> elements != NULL) {
        free(arr -> elements);
    }

    if (arr != NULL) {
        free(arr);
    }

}
