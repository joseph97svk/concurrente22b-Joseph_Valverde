#ifndef COMMONSTRUCTURES_HPP
#define COMMONSTRUCTURES_HPP

#include <iostream>

#include "commonStructures.hpp"

sharedData::sharedData(const size_t packageCount, const size_t producerAmount):
packageCount(packageCount),
totalProducersAmount(producerAmount),
receivedData(0) {
  this->hasData.resize(producerAmount);
}

size_t sharedData::getPackageCount() {
  this->canAccessCount.lock();
    size_t temp = this->packageCount;
    if (this->packageCount != 0) {
      (this->packageCount)--;
    }
  this->canAccessCount.unlock();
  return temp;
}

bool sharedData::getCanFinish() {
  this->canAccessFinishedAmount.lock();
    size_t ans = this->finishedProducedAmount;
  this->canAccessFinishedAmount.unlock();
  return ans == this->totalProducersAmount; 
}

void sharedData::notifyFinished() {
  std::cout << "done" << std::endl;
  this->canAccessFinishedAmount.lock();
    (this->finishedProducedAmount)++;
  this->canAccessFinishedAmount.unlock();
}


size_t sharedData::getProducerAmount() {
  return this->totalProducersAmount;
}
#endif