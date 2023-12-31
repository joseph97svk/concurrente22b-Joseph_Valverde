// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include <cstdlib>
#include <mutex>
#include <thread>

#include "common.hpp"

struct sharedData {
 private:
  size_t packageCount;

  size_t totalProducersAmount = 0;

  size_t finishedProducedAmount = 0;

  std::mutex canAccessCount;

  std::mutex canAccessFinishedAmount;

 public:
  sharedData(const size_t packageCount, const size_t producerAmount);

  DISABLE_COPY(sharedData);

  ~sharedData() = default;

  size_t getPackageCount();

  bool getCanFinish();

  void notifyFinished();
};
