// Copyright [2022] <Joseph Valverde>
#include "thread.hpp"

template <typename dataType>
class consumer: public thread<dataType> {
 public:
  consumer() {
    this -> myThread = nullptr;
  }

  DISABLE_COPY(consumer);

  void process(dataType data) override {
    simulationData* simulationData = (struct simulationData*) data;

    while (true) {
      simulationData->can_access_consumed_count.lock();
      if (simulationData->consumed_count < simulationData->unit_count) {
        ++simulationData->consumed_count;
      } else {
        simulationData->can_access_consumed_count.unlock();
        break;
      }
      simulationData->can_access_consumed_count.unlock();

      sem_wait(&simulationData->can_consume);

      size_t value = 0;

      simulationData->threadQueue.dequeue(&value);
      std::cout << "\tConsuming " << value << std::endl;

      usleep(1000 * random_between(simulationData->consumer_min_delay,
      simulationData->consumer_max_delay));
    }
  }
};
