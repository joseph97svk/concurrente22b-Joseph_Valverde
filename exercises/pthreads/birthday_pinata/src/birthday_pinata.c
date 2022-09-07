#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

enum error_state {
    EXIT_SUCCESS_ERROR,
    INVALID_HEALTH_HEALTH_INPUT_ERROR,
    INVALID_HEALTH_INPUT_ERROR,
    INVALID_ARGUMENT_AMOUNT_ERROR,
    MEMORY_ALLOCATION_FAILURE_FOR_SHARED_DATA,
    MEMORY_ALLOCATION_FAILURE_FOR_PRIVATE_DATA,
    MEMORY_ALLOCATION_FAILURE_FOR_THREADS,
};

typedef struct shared_data {
    pthread_mutex_t permision_to_strike;
    int32_t pinata_health;
    uint32_t threads_invited;

} shared_data_t;

typedef struct private_data { 
    shared_data_t* shared_data;
    u_int32_t thread_num;
} private_data_t;


int birthday_party (shared_data_t* shared_data);

void* hit_pinata (void* data);

int main(int argc, char* argv[]) {
    int error = EXIT_SUCCESS_ERROR;

    uint32_t threads_invited = sysconf(_SC_NPROCESSORS_ONLN);
    int32_t pinata_health = (int32_t) threads_invited;
    shared_data_t* shared_data = calloc(1, sizeof(shared_data_t));

    

    if (shared_data == NULL) {
        fprintf(stderr, "Memory allocation error for shared_datan!\n");
        return MEMORY_ALLOCATION_FAILURE_FOR_SHARED_DATA;
    }

    if (argc == 3) {
        if (sscanf(argv[1], "%u", &threads_invited) != 1  || sscanf(argv[2], "%i", &pinata_health) != 1) {
            fprintf(stderr, "Invalid amount of threads invited or health given\n");
            error = INVALID_HEALTH_HEALTH_INPUT_ERROR;
        }
           
        if (pinata_health <= 0) {
            fprintf(stderr, "Invalid pinata health given!\n");
            error = INVALID_HEALTH_INPUT_ERROR;
        }
    } else if (argc != 1) {
        fprintf(stderr, "Invalid amount of arguments given\n");
        error = INVALID_ARGUMENT_AMOUNT_ERROR;
    }

    if (error == EXIT_SUCCESS_ERROR) {
        error = pthread_mutex_init(&(shared_data -> permision_to_strike), /*attr*/NULL); 

        if (error == EXIT_SUCCESS) {
            shared_data -> pinata_health = pinata_health;
            shared_data -> threads_invited = threads_invited;

            error = birthday_party(shared_data);

            pthread_mutex_destroy(&(shared_data->permision_to_strike));
        }
    }

    free(shared_data);   

    return error;
}

int birthday_party (shared_data_t* shared_data) {

    int error = EXIT_SUCCESS;

    private_data_t* private_data = calloc(shared_data -> threads_invited, sizeof(private_data_t));

    if (private_data == NULL) {
        error = MEMORY_ALLOCATION_FAILURE_FOR_PRIVATE_DATA;
        goto end;
    }

    pthread_t* invited_threads = calloc(shared_data -> threads_invited, sizeof(size_t));

    if (invited_threads == NULL) {
        error = MEMORY_ALLOCATION_FAILURE_FOR_THREADS;
        goto end;
    }

    for (u_int32_t thread = 0; thread < shared_data -> threads_invited; thread++) {

        private_data[thread].shared_data = shared_data;
        private_data[thread].thread_num = thread + 1;

        error = pthread_create(&invited_threads[thread], NULL, hit_pinata, (void*) &private_data[thread]);

        if (error !=  EXIT_SUCCESS_ERROR) {
            goto end;  
        }
    }

    printf("Enjoy the party!\n");

    for (u_int32_t thread = 0; thread < shared_data -> threads_invited; thread++) {
        pthread_join(invited_threads[thread], NULL);
    }

    end:
    if (invited_threads != NULL) {
        free (invited_threads);
    }
    
    if (private_data != NULL) {
        free (private_data);
    }

    return error;
}

void* hit_pinata (void* data) {
    private_data_t* private_data = (private_data_t*) data;
    shared_data_t* shared_data = private_data -> shared_data;

    uint32_t hit_counter = 0;
    bool destroyed_pinata = false, pinata_is_alive = true;
    
    while (pinata_is_alive) {
        pthread_mutex_lock(&(shared_data -> permision_to_strike)); {
            // check if no other thread killed pinata before
            if (shared_data -> pinata_health != 0) {
                ++hit_counter;
                --(shared_data -> pinata_health);
        
                destroyed_pinata = shared_data -> pinata_health == 0;
            } // if pinata is gone, nothing to be done
            pinata_is_alive = shared_data -> pinata_health > 0;
        } pthread_mutex_unlock(&(shared_data -> permision_to_strike));

        // it takes time to swing again after having swung before
        usleep(50); 
    }

    if (!pinata_is_alive && destroyed_pinata) {
        printf("Thread %u/%u: %u hits, I broke the pinata\n",
        private_data -> thread_num,
        shared_data -> threads_invited, hit_counter);
    } else {
        printf("Thread %u/%u: %u hits\n", private_data -> thread_num,
        shared_data -> threads_invited, hit_counter);
    }

    return NULL;
}