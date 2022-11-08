#include <iostream>
#include <omp.h>

double* random_num_generator(int amount);
void serial_odd_even_sort(double* a, const int n);
void swap (double* a, double* b);
void print_numbers(double* numbers, int number_amount);
bool correctness_check(double* numbers, int number_amount);


int main (int argc, char* argv[]) {
  int number_amount = 0;
  if (argc >= 2) {
    number_amount = atoi(argv[1]);
  }

  int thread_amount = omp_get_max_threads();

  if (argc >= 3) {
    thread_amount = atoi(argv[2]);
  }

  double* numbers = NULL;


  numbers = random_num_generator(number_amount);

  std::cout << thread_amount << std::endl;

  #pragma omp parallel num_threads(thread_amount) \
    default(none) shared(numbers, number_amount, std::cout)  
  {

    #pragma omp critical(stdout)
    {
    std::cout << omp_get_thread_num() << "::" << omp_get_num_threads() << std::endl;
    }
    serial_odd_even_sort(numbers, number_amount);
  }
  

  std::cout << "ordered" << std::endl;

  //print_numbers(numbers, number_amount);

  std::cout <<
  (correctness_check(numbers, number_amount)? "All in order" : "Not in order")
  << std::endl;

  free (numbers);
}

double* random_num_generator(int amount) {
  size_t upper_limit = 99;
  double* numbers = (double*) calloc(amount, sizeof(double));

  for (int num = 0; num < amount; num++) {
    unsigned int seed = (unsigned int) num;
    double buffer = (double) rand_r(&seed);

    while (buffer > upper_limit) {
      buffer /= 2;
    }

    numbers[num] = buffer;
  }

  return numbers;
}

void serial_odd_even_sort(double* a, const int n) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      #pragma omp for
      for (int i = 1; i < n; i += 2) {
        if (a[i - 1] > a[i]) {
          swap(&a[i - 1], &a[i]);
        }
      }
    } else {
      #pragma omp for
      for (int i = 1; i < n - 1; i += 2) {
        if (a[i] > a[i + 1]) {
          swap(&a[i], &a[i + 1]);
        }
      }
    }
  }
}

void swap (double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

void print_numbers(double* numbers, int number_amount) {
  std::cout << "numbers:" << std::endl;
  for (int num = 0; num < number_amount; ++num) {
    std::cout << numbers[num] << std::endl;
  }
}

bool correctness_check(double* numbers, int number_amount) {
  for (int number = 1; number < number_amount; number++) {
    if (numbers[number - 1] > numbers[number]) {
      return false;
    }
  }

  return true;
}