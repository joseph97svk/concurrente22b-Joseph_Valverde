#include <random>

#include "common.hpp"

double random_between(double min, double max) {
  static std::random_device::result_type seed = std::random_device()();
  // This object generates randon numbers using the Mersenne-Twister algoritym
  static std::mt19937 randomEngine(seed);
  
  std::uniform_int_distribution<double> randomDistribution(min, max);

  return (double) randomDistribution(randomEngine);
}