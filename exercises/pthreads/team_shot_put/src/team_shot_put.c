#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

struct identity {
    int32_t team;
    int32_t player_num;
} identity_t;

int main(int argc, char* argv[]) {
    int64_t player_amount =  3;

    if (argc == 2) {
        if(sscanf(stdin, "%li", &player_amount) != 1) {
            fscanf(stderr, "Error: invalid input");
            return EXIT_FAILURE;
        } 
    } else if (argc > 2) {
        fscanf(stderr, "Error: invalid input");
        return EXIT_FAILURE;
    }


    // call players for the game
    pthread_t* teamA = malloc(player_amount * sizeof(pthread_t));
    pthread_t* teamB = malloc(player_amount * sizeof(pthread_t));

    int32_t* scoreboard = malloc(player_amount * sizeof(int32_t));

    if (teamA == NULL || teamB == NULL || scoreboard == NULL) {
        fscanf(stderr, "Error: one or both teams are missing from the field!\nThe scoreboad might have also gotten stolen!");
        // free threads
        free(teamA);
        free(teamB);
        free(scoreboard);
        
        return EXIT_FAILURE;
    }

    // make players shoot
    for (int64_t teammate = 0; teammate < player_amount; ++teammate) {
        // send player for team 1
        int32_t player_shoot_error = pthread_create(&teamA[teammate], NULL, player_shoot, (void*) teammate);

        // send player for team 2
        int32_t player_shoot_error = pthread_create(&teamB[teammate], NULL, player_shoot, (void*) teammate);
    }

    // collect the result of the players once they are done
    for (int64_t teammate = 0; teammate < player_amount; ++teammate) {
        double shot_team_a = 0;
        double shot_team_b = 0;
        pthread_join(teamA[teammate], &shot_team_a);
        pthread_join(teamB[teammate], &shot_team_b);

        // compare the collected results
        // set the scoreboard
        if (shot_team_a > shot_team_b) {
            scoreboard[teammate] = 1;
        } else if (shot_team_a < shot_team_b) {
            scoreboard[teammate] = 2;
        } else {
            scoreboard[teammate] = 0;
        }        
    }
    
    // report the results

    free(teamA);
    free(teamB);
    free(scoreboard);

    return EXIT_SUCCESS;
}

void* player_shoot (void* data) {
    // generate random number to be the score
    double number_generated = 0;

    size_t seed = (size_t*) data;
    for (int try = 0; try < 3; ++try) {
        seed++;
        double temp_number_generated = rand_r(&seed);

        if (temp_number_generated > number_generated) {
            number_generated = temp_number_generated;
        }
    }

    // print the score
    printf("%f", number_generated);
    // return the results

    return (void*)*(size_t*)&number_generated;
}