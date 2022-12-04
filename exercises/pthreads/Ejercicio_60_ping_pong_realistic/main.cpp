#include <iostream>
#include <random>

#include "mpi_wrapper/Mpi.hpp"

void pingPong(Mpi& mpi, int maxPoints, int rank, double playerAccuracy);

double getRandomNum(double min, double max);

int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);

    const int rank = mpi.getProcessNumber();
    const int count = mpi.getProcessCount();

    if (count != 2) {
      std::cerr << "Incorrect amount of players!" << std::endl
          << count << " given, but only amount allowed is 2!";
    }

    int maxPoints = 3;
    double playerAccuracy = 50.0;

    if (argc == 4) {
      maxPoints = std::stoi(argv[1]);
      playerAccuracy = rank == 0 ? std::stod(argv[2]) : std::stod(argv[3]);
    }

    pingPong(mpi, maxPoints, rank, playerAccuracy);

  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void pingPong(Mpi& mpi, int maxPoints, int rank, double playerAccuracy) {
  int other = (rank != 0? 0 : 1);
  int signal = 0;
  int score = 0;
  int scoreOtherPlayer = 0;

  for (int round = 0; round < 3; ++round) {
    int points = 0;
    int otherPoints = 0;
    int passesMade = 0;
    while (points < maxPoints && otherPoints < maxPoints) {
      passesMade++;
      double possibility = getRandomNum(0, 100);

      switch(rank) {
        case 0:

          if (possibility > playerAccuracy) {
            signal = 1;
            passesMade--;
          }

          mpi[other] << signal;
          mpi[other] >> signal;

          if (signal == 1) {
            if (otherPoints != maxPoints) {
              points++;
            }
            signal = 0;
          }
          break;
        case 1:
          mpi[other] >> signal;

          if (signal == 1) {
            if (otherPoints != maxPoints) {
              points++;
            }
            signal = 0;
          }

          if (possibility > playerAccuracy) {
            signal = 1;
            passesMade--;
          }

          mpi[other] << signal;
          break;
      }

      switch(rank) {
        case 0:
          mpi[other] >> otherPoints;
          mpi[other] << points;
          break;
        case 1:
          mpi[other] << points;
          mpi[other] >> otherPoints;
          break;
      }
    }

    // once three points are reached
    switch(rank) {
      case 0: {
        int plays = 0;
        mpi[other] >> plays;
        passesMade += plays;

        int winner = points > otherPoints ? 0 : 1;
        std::cout << round  + 1 << ": " << passesMade << " " <<
        winner << std::endl;

        if (winner == 0) {
          score++;
        } else {
          scoreOtherPlayer++;
        }
        break;
      }
        
      case 1:
        mpi[other] << passesMade;
        break;
    }
  }

  if (rank == 0) {
    int winner = score > scoreOtherPlayer ? rank : other;
    std::cout << winner << " wins "
        << (winner == rank ? score : scoreOtherPlayer) << " to "
        << (winner == rank ? scoreOtherPlayer : score) << std::endl;
  }
}

double getRandomNum(double min, double max) {
  std::random_device::result_type seed = std::random_device()();
  std::mt19937 randomEngine(seed);
  std::uniform_real_distribution<double> distribution(min, max);
  double randomNumber = distribution(randomEngine);
  return randomNumber;
}