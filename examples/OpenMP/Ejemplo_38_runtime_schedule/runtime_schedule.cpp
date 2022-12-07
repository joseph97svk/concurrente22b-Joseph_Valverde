// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

void print_mapping(const char* type, const std::vector<int>& mapping);

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  std::vector<int> mapping(iteration_count);

  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(iteration_count, mapping)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    mapping[iteration] = omp_get_thread_num();
  }

  #pragma omp single
  print_mapping("runtime   ", mapping);
}

void print_mapping(const char* type, const std::vector<int>& mapping) {
  std::cout << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
