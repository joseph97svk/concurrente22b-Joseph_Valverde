#include <iostream>
#include <omp.h>
#include <semaphore.h>
#include <string>
#include <random>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int CICLE_LIMIT = 20;

void philosopher(sem_t* chopsticks, int philosopherCount, int thinkingTime, int eatingTime);

int getRandomNumber (int max, int min);

int main (int argc, char* argv[]) {
  int philosopherCount = 0;
  int minTime = 0;
  int maxTime = 0;
  if (argc == 4) {
    philosopherCount = std::stoi(argv[1]);
    minTime = std::stoi(argv[2]);
    maxTime = std::stoi(argv[3]);
  } else {
    std::cerr << "Invalid amount of arguments given!" << std::endl;
    return EXIT_FAILURE;
  }

  if (philosopherCount < 2) {
    std::cerr << "Invalid amount of philosophers given!" << std::endl;
  }

  if ((maxTime < 0 && minTime < 0) || minTime > maxTime) {
    std::cerr << "Invalid times given!" << std::endl;
  }

  int eatingTime = 1000 * getRandomNumber(minTime, maxTime);
  int thinkingTime = 1000 * getRandomNumber(minTime, maxTime);

  sem_t* chopsticks = (sem_t*) calloc(philosopherCount, sizeof(sem_t));

  for (int chopstick = 0; chopstick < philosopherCount; ++chopstick) {
    sem_init(&chopsticks[chopstick], /*pshared*/ 0, /*value*/ 1);
  }

  #pragma omp parallel num_threads(philosopherCount)\
      default(none)\
      shared(chopsticks, philosopherCount, thinkingTime, eatingTime)
  philosopher(chopsticks, philosopherCount, thinkingTime, eatingTime);
  
}

void philosopher(sem_t* chopsticks, int philosopherCount,
    int thinkingTime, int eatingTime) {
  int philosopherId = omp_get_thread_num();

  int cicles = 0;
  while (true) {
    std::string thinkingMessage = "Philosopher " +
        std::to_string(philosopherId) + " thinking...\n";
    std::cout << thinkingMessage << std::endl;
    usleep(thinkingTime);

    if (philosopherId % 2 == 0) {
      sem_wait(&chopsticks[philosopherId]);
      sem_wait(&chopsticks[(philosopherId + 1) % philosopherCount]);
    } else {
      sem_wait(&chopsticks[(philosopherId + 1) % philosopherCount]);
      sem_wait(&chopsticks[philosopherId]);
    }

    std::string eatMessage = "Philosopher " +
        std::to_string(philosopherId) + " eating...\n";
    std::cout << eatMessage << std::endl;
    usleep(eatingTime);

    sem_post(&chopsticks[(philosopherId + 1) % philosopherCount]);
    sem_post(&chopsticks[philosopherId]);

    cicles++;
    // prevent program from looping forever but keeping eternal loop concept
    if (cicles == CICLE_LIMIT) {
      break;
    }
  }

  #pragma omp single
  std::cout << "philosophers done" << std::endl;
}

int getRandomNumber (int max, int min) {
  unsigned int seed = (unsigned int)(size_t) &max +
      (unsigned int)(size_t) &min;
  int randomNum = rand_r(&seed) % (max - min + 1) + min;
  return randomNum;
}