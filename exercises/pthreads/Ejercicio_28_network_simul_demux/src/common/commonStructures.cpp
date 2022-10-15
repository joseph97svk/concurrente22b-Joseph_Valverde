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
  //std::cout << "done" << std::endl;
  this->canAccessFinishedAmount.lock();
    (this->finishedProducedAmount)++;
  this->canAccessFinishedAmount.unlock();
}

size_t sharedData::getProducerAmount() {
  return this->totalProducersAmount;
}

Semaphore* sharedData::getReceivedData() {
  return &this->receivedData;
}

void sharedData::increaseHasData(int32_t pos) {
  this->canAccessDataAmount.lock();
    this->hasData[pos]++;
    this->inQueueAmount++;
  this->canAccessDataAmount.unlock();
}

bool sharedData::checkHasData(int32_t pos) {
  bool ans = false;
  this->canAccessDataAmount.lock();
    ans = this->hasData[pos];
  this->canAccessDataAmount.unlock();
  return ans;
}

void sharedData::reduceHasData(int32_t pos) {
  this->canAccessDataAmount.lock();
    this->hasData[pos]--;
    this->inQueueAmount--;
  this->canAccessDataAmount.unlock();
}

bool sharedData::leftInQueue() {
  bool ans = false;
  this->canAccessDataAmount.lock();
    ans = this->inQueueAmount == 0;
  this->canAccessDataAmount.unlock();
  return ans;
}

#endif