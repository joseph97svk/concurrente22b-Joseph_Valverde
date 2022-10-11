// Copyright [2022] <Joseph Valverde>
#include "thread.hpp"

template <typename dataType>
class producer: public thread<dataType> {
 public:
  producer() {
    this -> myThread = nullptr;
  }

  DISABLE_COPY(producer);

  void process(dataType data) override {progress on exercise 23
    simulationData* simulationData = (struct simulationData*) data;

    while (true) {
      size_t my_unit = 0;

      simulationData->can_access_next_unit.lock();
      if (simulationData->next_unit < simulationData->unprogress on exercise 23it_count) {
        my_unit = ++simulationData->next_unit;
      } else {
        simulationData->can_access_next_unit.unlock();
        break;
      }
      simulationData->can_access_next_unit.unlock();
      usleep(1000 * random_between(simulationData->producer_min_delay,
      simulationData->producer_max_delay));

      simulationData->threadQueue.enqueue(my_unit);
      std::cout << "Produced " << my_unit << std::endl;

      sem_post(&simulationData->can_consume);
    }
  }
};
