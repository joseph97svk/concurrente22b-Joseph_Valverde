#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void* player_shoot (void* data);

void erase_matrix (int64_t** arr, const int64_t rows);

int main(int argc, char* argv[]) {
    int64_t player_amount =  3;

    if (argc == 2) {
        if(sscanf(argv[1], "%li", &player_amount) != 1) {
            fscanf(stderr, "Error: invalid input");
            return EXIT_FAILURE;
        } 
    } else if (argc > 2) {
        fscanf(stderr, "Error: invalid input");
        return EXIT_FAILURE;
    }

    if (player_amount % 2 == 0) {
        fscanf(stderr, "Error: invalid player amount!");
        return EXIT_FAILURE;
    }
    // call players for the game
    pthread_t* teamA = malloc(player_amount * sizeof(pthread_t));
    pthread_t* teamB = malloc(player_amount * sizeof(pthread_t));

    int64_t teamA_score = 0, teamB_score = 0;

    if (teamA == NULL || teamB == NULL) {
        fscanf(stderr, "Error: one or both teams are missing from the field!\nThe scoreboad might have also gotten stolen!");

        // free arrays
        free(teamA);
        free(teamB);
        
        return EXIT_FAILURE;
    }

    int64_t** player_ids = malloc(2 * player_amount * sizeof(int64_t*));
    
    if (player_ids == NULL) {
        free (player_ids);
        free(teamA);
        free(teamB);
        return EXIT_FAILURE;
    }

    for (int64_t teammate = 0; teammate < player_amount * 2; ++teammate) {
        player_ids[teammate] = malloc(sizeof(int64_t) * 2);
        if (player_ids[teammate] == NULL) {
            erase_matrix(player_ids, teammate);
            return EXIT_FAILURE;
        }
    }

    // make players shoot
    for (int64_t teammate = 0; teammate < player_amount; ++teammate) {
        
        // send player for team 1
        player_ids[teammate][0] = 1;
        player_ids[teammate][1] = teammate + 1;
        int32_t player_shoot_errorA = pthread_create(&teamA[teammate], NULL, player_shoot, (void*) player_ids[teammate]);
        
        // send player for team 2
        player_ids[teammate + player_amount][0] = 2;
        player_ids[teammate + player_amount][1] = teammate + 1;
        int32_t player_shoot_errorB = pthread_create(&teamB[teammate], NULL, player_shoot, (void*) player_ids[teammate + player_amount]);

        if (player_shoot_errorA == EXIT_FAILURE || player_shoot_errorB == EXIT_FAILURE) {
            free (player_ids);
            free(teamA);
            free(teamB);
            fscanf(stderr, "Error: Teammate injured! The game was suspended");
            return EXIT_FAILURE;
        }
    }

    // collect the result of the players once they are done
    for (int64_t teammate = 0; teammate < player_amount; ++teammate) {
        double shot_team_a = 0;
        double shot_team_b = 0;
        pthread_join(teamA[teammate], (void*) &shot_team_a);
        // print the score

        pthread_join(teamB[teammate], (void*) &shot_team_b);
        // print the score

        // compare the collected results
        // set the scoreboard
        if (shot_team_a > shot_team_b) {
           teamA_score++;
        } else {
            teamB_score++;
        }        
    }

    int32_t winner = 2;
    if (teamA_score > teamB_score) {
        winner = 1;
    } 
    printf("result: %li:%li, team %i wins\n", teamA_score, teamB_score, winner);
   
    
    // report the results
    free(teamA);
    free(teamB);
    erase_matrix(player_ids, player_amount * 2);

    return EXIT_SUCCESS;
}

void erase_matrix (int64_t** arr, const int64_t rows) {
    if (arr == NULL) {
        return;
    }

    for (int64_t row = 0; row < rows; ++row) {
        free(arr[row]);
    }

    free(arr);
}

void* player_shoot (void* data) {
    // generate random number to be the score
    double number_generated = 0;

    int64_t* id_info = (int64_t*) data;

    int32_t seed = (int32_t)(id_info[1] + id_info[0]); 

    for (int try = 0; try < 3; ++try) {
        seed++;
        double temp_number_generated = rand_r(&seed);

        while (temp_number_generated > 25.0f) {
            temp_number_generated/=2;
        }

        if (temp_number_generated > number_generated) {
            number_generated = temp_number_generated;
        }
    }

    printf("%li.%li: best shot put %fm\n", id_info[0], id_info[1], number_generated);

    // return the results
    return (void*)*(size_t*)&number_generated;
}