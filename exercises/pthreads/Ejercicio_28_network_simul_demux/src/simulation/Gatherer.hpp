#include <vector>

#include "../common/common.hpp"
#include "../common/commonStructures.hpp"
#include "../prodcons/Queue.hpp"
#include "DispatcherTest.hpp"
#include "../prodcons/Consumer.hpp"

template <typename DataType>
class Gatherer: public Consumer<DataType> {
  DISABLE_COPY(Gatherer);

  sharedData* sharedProducerData;

  std::vector<Queue<DataType>*> fromQueues;

  std::vector<Queue<DataType>*> toQueues;

 public:
  Gatherer(sharedData* sharedProducerData = nullptr) :
  sharedProducerData(sharedProducerData) {}

  void createOwnQueues(size_t producerAmount) {
    this->fromQueues.resize(producerAmount);

    std::cout << producerAmount << std::endl;
    for (size_t producerQueue = 0;
    producerQueue < producerAmount;
    producerQueue++) {
      std::cout << producerQueue << std::endl;
      this->fromQueues[producerQueue] = new Queue<DataType>(MAXQUEUESIZE);
    }
    this->toQueues.resize(1);
  }

  inline Queue<DataType>* getProducingQueue(size_t producerNum) {
    return this->fromQueues[producerNum];
  }

  void registerRedirect(Queue<DataType>* toQueue) {
    this->toQueues[0] = toQueue;
  }

  int run() override {
    // Dispatch all the network messages we receive to their respective queues
    this->consumeForever();

    // If we exited from the forever loop, the finish message was received
    // For this simulation is OK to propagate it to all the queues
    
    this->toQueues[0]->push(NetworkMessage());

    return EXIT_SUCCESS;
  }

  void consumeForever() override{
    while (true) {
      std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
      this->sharedProducerData->receivedData.wait();
      std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

      size_t producerPos = 0;

      for (size_t producer = 0;
      producer < this->sharedProducerData->getProducerAmount();
      producer++) {
        if (this->sharedProducerData->hasData[producer] != 0) {
          std::cout << producer << "," << this->sharedProducerData->hasData[producer] << std::endl;
          producerPos = producer;
          break;
        }
      }

      std::cout << "(" << producerPos << ")" << std::endl;
      
      // Get the next data to consume, or block while queue is empty
      std::cout << "aaaaaaaaaa" << std::endl;
      const DataType& data = this->fromQueues[producerPos]->pop();
      std::cout << "bbbbbbbbbb" << std::endl;
      this->sharedProducerData->hasData[producerPos]--;
      
      std::cout << "::::::::" <<(data == this->stopCondition) << std::endl;
      // If data is the stop condition, stop the loop
      if ( data == this->stopCondition ) {
        std::cout << "here?" << std::endl;
        break;
      }
      // Process this data
      // std::cout << "[" << this->id << "]" << std::endl;
      this->consume(data);
    }
  }

  void consume(DataType data) override {
    this->toQueues[0]->push(data);
  }

};