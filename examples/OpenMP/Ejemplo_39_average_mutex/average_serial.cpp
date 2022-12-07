// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  std::vector<double> values;

  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  double sum = 0.0;

  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
  }

  const double average = values.size() ? sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
