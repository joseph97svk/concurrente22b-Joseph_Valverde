#include <cstdlib>
#include <semaphore>
#include <chrono>

#include "queue.hpp"

enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_UNIT_COUNT,
  ERR_PRODUCER_COUNT,
  ERR_CONSUMER_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

struct simulation_data {
  size_t unit_count;
  size_t producer_count;
  size_t consumer_count;

  queue<size_t> queue;

  size_t next_unit;
  std::mutex can_access_next_unit;

  double producer_min_delay;
  double producer_max_delay;
  double consumer_min_delay;
  double consumer_max_delay;


  std::mutex can_access_qeueue;
  size_t consumed_count;

  public:
  simulation_data (size_t unit_count = 0,
  size_t producer_count = 0, size_t consumer_count = 0,
  double producer_min_delay = 0,
  double producer_max_delay = 0,
  double consumer_min_delay = 0,
  double consumer_max_delay = 0):
  unit_count(unit_count),
  producer_count(producer_count),
  consumer_count(consumer_count),
  queue(),
  producer_min_delay(producer_min_delay),
  producer_max_delay(producer_max_delay),
  consumer_min_delay(consumer_min_delay),
  consumer_max_delay(consumer_max_delay),
  can_access_next_unit(),
  next_unit(0),
  can_access_qeueue(),
  consumed_count(0){

  }

  ~simulation_data() {
  }

  

};

double random_between(double min, double max);