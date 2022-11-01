#include <iostream>

double* random_num_generator(int amount);
double get_random_number();
void serial_odd_even_sort(double* a, const int n);
void swap (double* a, double* b);
void print_numbers(double* numbers, int number_amount);

int main (int argc, char* argv[]) {
  int number_amount = 0;
  if (argc >= 2) {
    number_amount = atoi(argv[1]);
  }

  double* numbers = random_num_generator(number_amount);

  print_numbers(numbers, number_amount);

  serial_odd_even_sort(numbers, number_amount);

  std::cout << "ordered" << std::endl;

  print_numbers(numbers, number_amount);

  free (numbers);
}

double* random_num_generator(int amount) {
  size_t lower_limit = 0, upper_limit = 99;
  double* numbers = (double*) calloc(amount, sizeof(double));
  for (int num = 0; num < amount; num++) {
    unsigned int seed = (unsigned int) num;
    numbers[num] = (double) (rand_r(&seed) % (upper_limit - lower_limit + 1)) + lower_limit;
  }

  return numbers;
}

double get_random_number() {
  
}

void serial_odd_even_sort(double* arr, const int n) {
  bool isSorted = false; // Initially array is unsorted
 
  while (!isSorted) {
    isSorted = true;

    // Perform Bubble sort on odd indexed element
    for (int i = 1; i <= n - 2; i = i + 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
      }
    }

    // Perform Bubble sort on even indexed element
    for (int i = 0; i <= n - 2; i = i + 2) {
      if (arr[i] > arr[i + 1]) {
        swap(&arr[i], &arr[i + 1]);
        isSorted = false;
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