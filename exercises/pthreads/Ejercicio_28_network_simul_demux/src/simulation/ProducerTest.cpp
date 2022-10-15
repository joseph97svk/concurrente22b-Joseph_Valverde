/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "ProducerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"
#include "../common/commonStructures.hpp"

ProducerTest::ProducerTest(size_t packageCount, int productorDelay
  , size_t consumerCount, sharedData* sharedPackageCount, 
  size_t id)
  : packageCount(packageCount)
  , productorDelay(productorDelay)
  , consumerCount(consumerCount),
  sharedPackageCount(sharedPackageCount),
  id(id) {
}

int ProducerTest::run() {
  // Produce each asked message
  while (this->getPackageCount() > 0) {
    this->produce(this->createMessage(this->packageCount));
    (this->packagesProduced)++;
    this->sharedPackageCount->increaseHasData(this->id);
    this->sharedPackageCount->getReceivedData()->signal();
  }

  this->sharedPackageCount->notifyFinished();

  //std::cout << "dony" << std::endl;

  // Produce an empty message to communicate we finished
  if (this->sharedPackageCount->getCanFinish()) {
    this->produce(NetworkMessage());
    this->sharedPackageCount->increaseHasData(this->id);
    this->sharedPackageCount->getReceivedData()->signal();
  }

  // Report production is done
  Log::append(Log::INFO, "Producer", std::to_string(this->packagesProduced)
    + " menssages sent");
  return EXIT_SUCCESS;
}

size_t ProducerTest::getPackageCount() {
  this->packageCount = this->sharedPackageCount->getPackageCount();
  return this->packageCount;
}

NetworkMessage ProducerTest::createMessage(size_t index) const {
  // Source is always 1: this producer
  const uint16_t source = 1;
  // Target is selected by random
  const uint16_t target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount));
  // IMPORTANT: This simulation uses sleep() to mimics the process of
  // producing a message. However, you must NEVER use sleep() for real projects
  Util::sleepFor(this->productorDelay);
  // Create and return a copy of the network message
  return NetworkMessage(target, source, index);
}


