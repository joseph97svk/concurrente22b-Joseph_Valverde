#include <stdint.h>

struct goldbach_element;

typedef struct goldbach_element goldbach_element_t;

struct Integer;

typedef struct Integer integer_t;

struct goldbach_arr {
    integer_t* length;
    goldbach_element_t** elements;
};

typedef struct goldbach_arr goldbach_arr_t;

int32_t goldbach_arr_init(goldbach_arr_t* arr);

int32_t goldbach_add_num(goldbach_arr_t* arr, const int64_t num); 

int32_t goldbach_add_sum(goldbach_arr_t* arr, const int64_t* const sum, const int64_t position);

int64_t goldbach_get_arr_length(goldbach_arr_t* arr);

int64_t goldbach_get_current_number(goldbach_arr_t* arr, const int64_t position);

int64_t goldbach_get_sums_amount(goldbach_arr_t* arr, const int64_t position);

/**
 * @brief Returns an array with the numbers of the sum
 * @details sum must be dealocated afterwards to prevent memory leak
 * @param arr 
 * @param size 
 * @param num_position 
 * @param sum_position 
 * @return int* 
 */
int64_t* goldbach_get_sum(const goldbach_arr_t* const arr, int64_t* size, const int64_t num_position, const int64_t sum_position);

void goldbach_delete_list(goldbach_arr_t* arr);

