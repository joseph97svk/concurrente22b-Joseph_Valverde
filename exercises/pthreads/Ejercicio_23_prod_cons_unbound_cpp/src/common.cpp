// Copyright [2022] <Joseph Valverde>
#include <random>

#include "common.hpp"


double random_between(double min, double max) {
  u_int32_t seed = ((u_int32_t)&min + (u_int32_t)&min)/2;
  double ans = rand_r(&seed);

  while (ans > max && ans > min) {
    ans /= 2;
  }

  return ans;

  /*
  static std::random_device::result_type seed = std::random_device()();
  // This object generates randon numbers using the Mersenne-Twister algoritym
  static std::mt19937 randomEngine(seed);
  
  std::uniform_int_distribution<double> randomDistribution(min, max);

  return (double) randomDistribution(randomEngine); */
}
