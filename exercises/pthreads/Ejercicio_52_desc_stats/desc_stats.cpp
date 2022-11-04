#include <iostream>
#include <omp.h>

double* readNumbers(size_t* numberAmount);

int main (int argc, char* argv[]) {
  int thread_amount = omp_get_max_threads();

  if (argc >= 2) {
    thread_amount = atoi(argv[1]);
  }

  size_t numberAmount = 0;
  double* numbers = readNumbers(&numberAmount);

}

double* readNumbers(size_t* numberAmount) {
  size_t capacity = 20, localNumberAmount = 0;
  double* numbers = (double*) calloc(capacity, sizeof(double));

  while (std::cin >> numbers[localNumberAmount]) {
    localNumberAmount++;

    if (localNumberAmount == capacity) {
      capacity *= 10;

      double* temp = (double*) realloc(numbers, capacity * sizeof(double));
      if (temp == nullptr) {
        return nullptr;
      } else {
        numbers = temp;
      }
    }
  }

  if (capacity != localNumberAmount) {

  }
}