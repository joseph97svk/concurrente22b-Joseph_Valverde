#include <iostream>

#include "mpi_wrapper/Mpi.hpp"

void pingPong(int playsAmount, int rank, Mpi& mpi);

int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);

    const int rank = mpi.getProcessNumber();
    const int count = mpi.getProcessCount();

    if (count != 2) {
      std::cerr << "Incorrect amount of players!" << std::endl
          << count << " given, but only amount allowed is 2!";
    }

    int playsAmount = 10;

    if (argc == 2) {
      playsAmount = std::stoi(argv[1]);
      if (playsAmount < 10) {
        playsAmount = 10;
      }
    }

    pingPong(playsAmount/2, rank, mpi);
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void pingPong(int playsAmount, int rank, Mpi& mpi) {
  int other = rank == 0? 1 : 0;

  int signal = 0;

  for (int play = 0; play < playsAmount; ++play) {
    switch(rank) {
      case 0:
        mpi[other] << signal;
        std::cout << "Player " << rank << " hits the ball!" << std::endl;
        mpi[other] >> signal;
        break;
      case 1:
        mpi[other] << signal;
        std::cout << "Player " << rank << " hits the ball!" << std::endl;
        mpi[other] >> signal;
        break;
    }
  }

  if (rank == 0) {
    std::cout << "Perfect game is over" << std::endl;
  }
}