// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <vector>

#include "common.hpp"
#include "../prodcons/Semaphore.hpp"

struct sharedData {
 private:
  size_t packageCount;

  size_t totalProducersAmount = 0;

  size_t finishedProducedAmount = 0;

  std::mutex canAccessCount;

  std::mutex canAccessFinishedAmount;

  std::mutex canAccessDataAmount;

  std::vector<int32_t> hasData;

  Semaphore receivedData;

  int32_t inQueueAmount = 0;

 public:
  sharedData(const size_t packageCount, const size_t producerAmount);

  DISABLE_COPY(sharedData);

  ~sharedData() = default;

  size_t getPackageCount();

  bool getCanFinish();

  void notifyFinished();

  size_t getProducerAmount();

  Semaphore* getReceivedData();

  void increaseHasData(int32_t pos);

  bool checkHasData(int32_t pos);

  void reduceHasData(int32_t pos);

  bool leftInQueue();
};
