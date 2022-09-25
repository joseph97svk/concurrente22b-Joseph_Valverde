// Copyright [2022] <Joseph Valverde>
#include <thread>
#include <memory>
#include <vector>

#include "common.hpp"
#include "producer.hpp"
#include "consumer.hpp"

class simulation {
 private:
  struct simulationData* simulationData;

  std::vector<std::shared_ptr<thread<struct simulationData*>>> producers;
  std::vector<std::shared_ptr<thread<struct simulationData*>>> consumers;

  size_t thread_amount;

 public:
  simulation():
  simulationData(nullptr),
  thread_amount(0) {
  }

  ~simulation() {
    delete(this -> simulationData);
  }

  void run(int argc, char* argv[]);

 private:
  void analyzeArguments(int argc, char* argv[]);
  void createConsumersProducers();
  void createThreads(size_t threadType);
  void addThread(size_t threadType);
  void joinThreads();
};
