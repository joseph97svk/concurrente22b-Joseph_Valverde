#include <iostream>
#include <omp.h>
#include <climits>
#include <cmath>

struct Results {
  double minimum = INT_MAX;
  double max = INT_MIN;
  double totalSum = 0;
  double median = 0;
  double average = 0;
  double standardDeviationAValue = 0;
  double standardDeviationBValue = 0;
  double standardDeviation = 0;
};

double* readNumbers(size_t* numberAmount);
void calculateAll(double* numbers, const int numberAmount,
Results* results);
void odd_even_sort(double* a, const int n, int thread_amount);

int main (int argc, char* argv[]) {
  int thread_amount = omp_get_max_threads();

  if (argc >= 2) {
    thread_amount = atoi(argv[1]);
  }

  size_t numberAmount = 0;
  double* numbers = readNumbers(&numberAmount);

  Results results;

  #pragma omp parallel num_threads(thread_amount) \
    default(none) shared(numbers, numberAmount, results, thread_amount)  
  {
    calculateAll(numbers, numberAmount, &results);

    odd_even_sort(numbers, numberAmount, thread_amount);
  }

  results.median = ((numberAmount % 2 != 0)? 
  numbers[numberAmount/2] : 
  ((numbers[numberAmount/2] + numbers[numberAmount/2 + 1])/2));
 
  results.average = results.totalSum/numberAmount;

  double standardDeviationSumatory =
  (results.standardDeviationAValue +
  (results.standardDeviationBValue * results.average) +
  ((results.average * results.average) * numberAmount));

  results.standardDeviation = sqrt(standardDeviationSumatory/(numberAmount));

  std::cout << "RESULTS:" << std::endl;
  std::cout << "Minimum: " << results.minimum << std::endl;
  std::cout << "Median: " << results.median << std::endl; 
  std::cout << "Average: " << results.average << std::endl;
  std::cout << "Standard Deviation: " << results.standardDeviation << std::endl;
  std::cout << "Maximum: " << results.max << std::endl;

  free(numbers);
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
    capacity = localNumberAmount;
    double* temp = (double*) realloc(numbers, capacity * sizeof(double));
    if (temp == nullptr) {
      return nullptr;
    } else {
      numbers = temp;
    }
  }

  *numberAmount = localNumberAmount;
  
  return numbers;
}

void calculateAll(double* numbers, const int numberAmount,
Results* results) {
  double localMax = INT_MIN, 
  localMin = INT_MAX,
  localSumCount = 0, 
  localSDAVal = 0,
  localSDBVal = 0;
  
  #pragma omp for 
  for (int number = 0; number < numberAmount; ++number) {
    if (numbers[number] > localMax) {
      localMax = numbers[number];
    }
    if (numbers[number] < localMin) {
      localMin = numbers[number];
    }

    localSumCount+= numbers[number];

    localSDAVal += (numbers[number] * numbers[number]);
    localSDBVal += -(numbers[number] * 2);
  }

  #pragma omp critical
  {
    if (localMax > results->max) {
      results->max = localMax;
    }
    if (localMin < results->minimum) {
      results->minimum = localMin;
    }
    (results->totalSum) += localSumCount;

    (results->standardDeviationAValue) += localSDAVal;
    (results->standardDeviationBValue) += localSDBVal;
  } 
  
}

void swap (double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

void odd_even_sort(double* a, const int n, int thread_amount) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      #pragma omp parallel for num_threads(thread_amount) \
      default(none) shared(a, n, std::cout) 
      for (int i = 1; i < n; i += 2) {
        if (a[i - 1] > a[i]) {
          swap(&a[i - 1], &a[i]);
        }
      }
    } else {
      #pragma omp parallel for num_threads(thread_amount) \
      default(none) shared(a, n, std::cout) 
      for (int i = 1; i < n - 1; i += 2) {
        if (a[i] > a[i + 1]) {
          swap(&a[i], &a[i + 1]);
        }
      }
    }
  }
}