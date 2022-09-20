#include <thread>
#include <memory>

#include "common.hpp"
#include "producer.hpp"
#include "consumer.hpp"


class simulation {
  private:
  struct simulation_data* simulation_data;
  
  std::vector<std::shared_ptr<thread>> producers;
  std::vector<std::shared_ptr<thread>> consumers;
  
  size_t thread_amount;
  producer producer;
  consumer consumer;

  public:
  simulation():
  simulation_data(nullptr),
  thread_amount(0){
  }

  ~simulation(){

  }

  void run (int argc, char* argv[]);

  private:

  void analyzeArguments(int argc, char* argv[]);
  void createConsumersProducers();
  void createThreads ();
  void joinThreads();
};