/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"
#include "AssemblerTest.hpp"


const char* const usage =
  "Usage: prodcons packages consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  consumers   number of consumer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  for (ProducerTest* producer : this->producers )
    delete producer;

  delete this->gatherer;

  for ( ConsumerTest* consumer : this->consumers )
    delete consumer;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  sharedData* sharedPackageCount = new sharedData(this->packageCount, this->producerCount);

  this->gatherer = new Gatherer<NetworkMessage>(sharedPackageCount);
  this->gatherer->createOwnQueues(this->producerCount);
  // Create each producer
  this->consumers.resize(this->consumerCount);
  // TODO: -1
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue(this->queueCapacities);
  }

  // Create each producer
  this->producers.resize(this->producerCount);
  // Create objects for the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index] = new ProducerTest(0, this->productorDelay
    , this->consumerCount, sharedPackageCount, index);
    // Communicate simulation objects
    // Producer push network messages to the dispatcher queue
    this->producers[index]->setProducingQueue(this->gatherer->getProducingQueue(index));
  }

  // Dispatcher delivers to each consumer, and they should be registered
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->gatherer->registerRedirect(this->consumers[index]->getConsumingQueue());
  }

  // Start the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->startThread();
  }

  this->gatherer->startThread();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->startThread();
  }

  // Wait for objets to finish the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->waitToFinish();
  }

  std::cout << "producers done" << std::endl;

  this->gatherer->waitToFinish();

  std::cout << "gatherer done" << std::endl;
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }

  delete(sharedPackageCount);

  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 5 + 1 arguments are mandatory
  if ( argc < 6 ) {
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  this->productorDelay = std::atoi(argv[index++]);
  this->dispatcherDelay = std::atoi(argv[index++]);
  this->consumerDelay = std::atoi(argv[index++]);

  if (argc == 7) {
    this->queueCapacities = std::atoll(argv[index++]);
  }

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}
