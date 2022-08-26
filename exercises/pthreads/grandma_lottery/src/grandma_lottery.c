#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* grandmother_orders (void* value);
void* fetch_lottery (void* lottery_number);

int main (void) {
    pthread_t grandmother_thread;

    // grandmother thread
    int grandmother_error = pthread_create(&grandmother_thread, /*attr*/ NULL, grandmother_orders, NULL);

    // join the grandmother thread
    if (grandmother_error == EXIT_SUCCESS) {
        printf("Kids, go buy me some lottery please\n");

        pthread_join(grandmother_thread, NULL);
    } else {
        fprintf(stderr, "Error: could not create \"grandmother\" main thread");
    }

    return grandmother_error;
}

void* grandmother_orders (void* value) {
    // join uses **, so passing address of a pointer should give a **
    size_t* first_value, second_value; 

    pthread_t grandchild_a_thread, grandchild_b_thread;

    size_t pseudoseed_a = (size_t) &grandchild_a_thread, pseudoseed_b = (size_t) &grandchild_b_thread;

    // create threads, no parameter to be passed
    int grandchild_a_error = pthread_create(&grandchild_a_thread, NULL, fetch_lottery, (void*) pseudoseed_a),
        grandchild_b_error = pthread_create(&grandchild_b_thread, NULL, fetch_lottery, (void*) pseudoseed_b);

    if (grandchild_a_error == EXIT_SUCCESS && grandchild_b_error == EXIT_SUCCESS) {

        // join threads, return to the values
        if (pthread_join(grandchild_a_thread, (void**) &first_value) == EXIT_SUCCESS &&
        pthread_join(grandchild_b_thread, (void**) &second_value) == EXIT_SUCCESS) {
            // print the values as are, return from threads as ** saved as an address on the pointer 
            printf("The grandchildren fetched the numbers: %lu and %lu\n", (size_t)first_value, (size_t)second_value);
        } else {
            printf("Error: 1 or more threads could not be joined");
        }
        
    } else {
        fprintf(stderr, "Error: could not create grandchilden threads.\nLottery could not be fetched due to inclement weather");
    }

    return value;
}

#define VARIANT 2

void* fetch_lottery (void* lottery_number) {
    (void) lottery_number; // supressing unused parameter warning
    size_t lower_limit = 0, upper_limit = 99;

    // create random number
    unsigned int seed = (size_t) lottery_number;
    
    size_t lottery_number_fetched = (size_t) (rand_r(&seed) % (upper_limit - lower_limit + 1)) + lower_limit;

    // kids bought the number and report it
    printf("Grandma! I bought %lu!\n", lottery_number_fetched);

    #if VARIANT == 1 //for first case (address of purchased number)
        return (void*) &lottery_number_fetched; 
        /* Will not work
         * The address is to a local variable which will be lost after the return
        */
    #endif

    #if VARIANT == 2 //for second case (fake memory address)
        return (void*) lottery_number_fetched;
        /* Works
         * Fake address, use it as the number saved, not an address
        */
    #endif
}