#include <thread>

#include "common.hpp"


class simulation {
  private:
  struct simulation_data* simulation_data;

  std::thread* producerThreads;
  std::thread* consumerThreads;
  size_t thread_amount;

  public:
  simulation():
  simulation_data(nullptr),
  producerThreads(nullptr),
  consumerThreads(nullptr), 
  thread_amount(0){
  }

  ~simulation(){

  }

  void run (int argc, char* argv[]);

  private:

  void analyzeArguments(int argc, char* argv[]);
  void createConsumersProducers();
  void joinThreads();
};