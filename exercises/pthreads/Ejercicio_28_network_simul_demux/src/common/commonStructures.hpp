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

 public:

  Semaphore receivedData;

  std::vector<int32_t> hasData;

  sharedData(const size_t packageCount, const size_t producerAmount);

  DISABLE_COPY(sharedData);

  ~sharedData() = default;

  size_t getPackageCount();

  bool getCanFinish();

  void notifyFinished();

  size_t getProducerAmount();
};