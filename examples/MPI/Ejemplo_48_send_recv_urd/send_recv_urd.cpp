// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#define fail(msg) throw std::runtime_error(msg)

const size_t MESSAGE_CAPACITY = 512;

void greet(int process_number, int process_count, const char* process_hostname);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);

    try {
      greet(process_number, process_count, process_hostname);
    } catch (const std::runtime_error& exception) {
      std::cerr << "error: " << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }

    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void greet(int process_number, int process_count
  , const char* process_hostname) {
  std::stringstream buffer;
  buffer << "Hello from main thread of process " << process_number
    << " of " << process_count << " on " << process_hostname;

  if (process_number != 0) {
    const std::string& message = buffer.str();
    if (MPI_Send(message.data(), message.length(), MPI_CHAR, /*target*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fail("could not send message");
    }
  } else {
    std::cout << buffer.str() << std::endl;
    for (int current = 1; current < process_count; ++current) {
      std::vector<char> message(MESSAGE_CAPACITY);
      if (MPI_Recv(&message[0], MESSAGE_CAPACITY, MPI_CHAR, MPI_ANY_SOURCE
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive message");
      }
      std::cout << &message[0] << std::endl;
    }
  }
}
