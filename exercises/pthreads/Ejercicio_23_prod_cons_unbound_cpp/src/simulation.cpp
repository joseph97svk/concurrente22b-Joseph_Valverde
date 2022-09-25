// Copyright [2022] <Joseph Valverde>
#include <iostream>
#include <sstream>

#include "simulation.hpp"


const bool PRODUCER = 0;
const bool CONSUMER = 1;

void simulation::run(int argc, char* argv[]) {
  this->analyzeArguments(argc, argv);

  this->createConsumersProducers();
}

void simulation::analyzeArguments(int argc, char* argv[]) {
  this->simulationData = new class simulationData();
  if (argc == 8) {
    std::istringstream argument1Buffer(argv[1]);
    argument1Buffer >> simulationData -> unit_count;
    std::istringstream argument2Buffer(argv[2]);
    argument2Buffer >> simulationData -> producer_count;
    std::istringstream argument3Buffer(argv[3]);
    argument3Buffer >> simulationData -> consumer_count;
    std::istringstream argument4Buffer(argv[4]);
    argument4Buffer >> simulationData -> producer_min_delay;
    std::istringstream argument5Buffer(argv[5]);
    argument4Buffer >> simulationData -> producer_max_delay;
    std::istringstream argument6Buffer(argv[6]);
    argument4Buffer >> simulationData -> consumer_min_delay;
    std::istringstream argument7Buffer(argv[7]);
    argument4Buffer >> simulationData -> consumer_max_delay;
  } else {
  }
}

void simulation::createConsumersProducers() {
  this->createThreads(PRODUCER);
  this->createThreads(CONSUMER);

  this->joinThreads();
}

void simulation::addThread(size_t threadType) {
  std::shared_ptr<thread<struct simulationData*>> temp;

  switch (threadType) {
    case PRODUCER:
      temp = std::shared_ptr<thread<struct simulationData*>>
      (new class producer<struct simulationData*>());
      this -> producers.emplace_back(temp);
      break;
    case CONSUMER:
      temp = std::shared_ptr<thread<struct simulationData*>>
      (new class consumer<struct simulationData*>());
      this -> consumers.emplace_back(temp);
      break;
    default:
      break;
  }

  temp -> initThread(this -> simulationData);
}

void simulation::createThreads(size_t threadType) {
  switch (threadType) {
    case PRODUCER:
      this->producers.reserve(this->simulationData->producer_count);
      break;
    case CONSUMER:
      this->consumers.reserve(this->simulationData->consumer_count);
      break;
    default:
      break;
  }

  int64_t max = this->simulationData->consumer_count;

  if (threadType == PRODUCER) {
    max = this->simulationData->producer_count;
  }

  for (int thread = 0; thread < max; ++thread) {
    this->addThread(threadType);
  }
}

void simulation::joinThreads() {
  for (size_t thread = 0;
  thread < this->simulationData->producer_count;
  ++thread) {
    this->producers[thread]->join();
  }

  for (size_t thread = 0;
  thread < this->simulationData->producer_count;
  ++thread) {
    this->consumers[thread]->join();
  }
}
