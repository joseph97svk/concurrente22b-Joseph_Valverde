#include <stdint.h>

/**
 * @brief enum for error handling
 * 
 */
enum golbach_arr_error {
    error_element_memory_allocation_failure = 120,
    error_goldbach_arr_elements_memory_allocation_failure,
    error_adding_sum_failure_invalid_position_or_given_sum,
    error_adding_sum_sums_memory_allocation_failure,
    error_adding_sum_current_sum_memory_allocation_failure,
    error_adding_sum_provided_sum_invalid_size,
}; 

/**
 * @brief Dynamic array holding the numbers and sums (once processed)
 * according to the goldbach conjecture
 * 
 * @details Holds an array with goldbach elements which hold necesary information
 * MUST be created to use
 * MUST be destroyed to prevent memory leaks
 */
struct goldbach_arr;

/**
 * @brief @see goldbach_arr
 */
typedef struct goldbach_arr goldbach_arr_t;

/**
 * @brief Returns pointer of created and initialized goldbach_arr
 * @details Goldbach_arr allocated in memory,
 * MUST call goldbach_arr_destroy to prevent memory leaks
 * 
 * @return goldbach_arr_t* 
 */
goldbach_arr_t* goldbach_arr_create();

/**
 * @brief Adds a number to dynamic array goldbach_arr
 * 
 * @param arr where the number will be stored
 * @param num that will be stored in goldbach_arr
 * @return int32_t success-error state
 */
int32_t goldbach_add_num(goldbach_arr_t* arr, const int64_t num); 

/**
 * @brief Adds an array of the numbers of an addition
 * @details Copies numbers from given array into arr,
 * no need to dinamically allocate and given arr can be discarded
 * Copies amount of numbers according to number in position of arr
 * @param arr where the sum will be stored
 * @param sum array of sum to be copied into arr
 * @param position within arr of where the sum will be stored
 * note: Invalid positions will return error and nothing will be copied
 * @return int32_t success-error state
 */
int32_t goldbach_add_sum(goldbach_arr_t* arr, 
const int64_t* const sum, const int64_t position);

/**
 * @brief returns the count of elements or numbers in arr
 * 
 * @param arr goldbach array of numbers and processed sums
 * @return int64_t 
 */
int64_t goldbach_get_arr_count(goldbach_arr_t* arr);

/**
 * @brief returns the number at a given position
 * @details invalid position will return 0
 * @param arr array from where number is going to be fetched
 * @param position where the number is going to be fetched
 * @return int64_t 
 */
int64_t goldbach_get_current_number(goldbach_arr_t* arr, const int64_t position);

/**
 * @brief returns the amount of sums located in a given position of arr
 * @details invalid position will return -1
 * @param arr @see goldbach_array
 * @param position of number whose sums amount is to be fetched
 * @return int64_t 
 */
int64_t goldbach_get_sums_amount(goldbach_arr_t* arr, const int64_t position);

/**
 * @brief returns the total amount of sums stored among all the numbers in the array
 * 
 * @param arr @see goldbach_array
 * @return int64_t 
 */
int64_t goldbach_arr_get_total_sums_amount(goldbach_arr_t* arr);

/**
 * @brief Returns an array with the numbers of the sum
 * @details sum must be dealocated afterwards to prevent memory leak
 * invalid positions return null pointer
 * failure to allocate array for sum will return null pointer
 * @param arr the goldbach_arr from which the sum is fetched
 * @param size pointer to which the size of the returned sum will be stored
 * @param num_position position of the number
 * @param sum_position position from among the sums for the given number
 * @return int* 
 */
int64_t* goldbach_get_sum(const goldbach_arr_t* const arr, int64_t* size, 
const int64_t num_position, const int64_t sum_position);

/**
 * @brief deallocates all memory allocated for goldbach_arr
 * 
 * @param arr @see goldbach_arr
 */
void goldbach_arr_destroy(goldbach_arr_t* arr);

