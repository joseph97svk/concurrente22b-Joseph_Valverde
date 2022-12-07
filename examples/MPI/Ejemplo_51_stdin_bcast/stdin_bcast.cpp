// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg)

void process_values(int process_number, const char* process_hostname);

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
      const double start_time = MPI_Wtime();
      process_values(process_number, process_hostname);
      const double elapsed = MPI_Wtime() - start_time;
      std::cout << process_hostname << ":" << process_number
        << ".m: elapsed time " << elapsed << "s" << std::endl;
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

void process_values(int process_number, const char* process_hostname) {
  std::vector<double> values;
  size_t value_count = 0;

  if (process_number == 0) {
    double value = 0.0;
    while (std::cin >> value) {
      values.push_back(value);
    }

    value_count = values.size();
  }

  static_assert(sizeof(value_count) == sizeof(uint64_t)
    , "update MPI_Send data type to match your architecture");
  if (MPI_Bcast(&value_count, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fail("could not broadcast value count");
  }

  values.resize(value_count);

  if (MPI_Bcast(&values[0], value_count, MPI_DOUBLE, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fail("could not broadcast values");
  }

  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
