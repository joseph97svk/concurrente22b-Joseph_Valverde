#include <vector>

#include "../common/common.hpp"
#include "../common/commonStructures.hpp"
#include "../prodcons/Queue.hpp"
#include "DispatcherTest.hpp"
#include "../prodcons/Consumer.hpp"

template <typename DataType>
class Demux: public Consumer<DataType> {
  DISABLE_COPY(Demux);

  sharedData* sharedProducerData;

  std::vector<Queue<DataType>*> fromQueues;

  std::vector<Queue<DataType>*> toQueues;

  Semaphore canConsume;

 public:
  Demux(sharedData* sharedProducerData = nullptr) :
  sharedProducerData(sharedProducerData),
  canConsume(0) {}

  void createOwnQueues(size_t producerAmount) {
    this->fromQueues.resize(producerAmount);

    std::cout << producerAmount << std::endl;
    for (size_t producerQueue = 0;
    producerQueue < producerAmount;
    producerQueue++) {
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
    bool done = false;
    while (true) {
      if (done && this->sharedProducerData->leftInQueue()) {
        break;
      }

      //std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
      this->sharedProducerData->getReceivedData()->wait();
      //std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

      size_t producerPos = 0;

      for (size_t producer = 0;
      producer < this->sharedProducerData->getProducerAmount();
      producer++) {
        if (this->sharedProducerData->checkHasData(producer)) {
          //std::cout << producer << "," << this->sharedProducerData->checkHasData(producer) << std::endl;
          producerPos = producer;
          break;
        }
      }

      //std::cout << "(" << producerPos << ")" << std::endl;
      
      // Get the next data to consume, or block while queue is empty
      //std::cout << "aaaaaaaaaa" << std::endl;
      const DataType& data = this->fromQueues[producerPos]->pop();
      //std::cout << "bbbbbbbbbb" << std::endl;
      this->sharedProducerData->reduceHasData(producerPos);
      
      //std::cout << "::::::::" <<(data == this->stopCondition) << std::endl;
      // If data is the stop condition, stop the loop
      if ( data == this->stopCondition ) {
        //std::cout << "here?" << std::endl;
        done = true;
        continue;
      }
      // Process this data
      this->consume(data);
    }
  }

  void consume(DataType data) override {
    this->toQueues[0]->push(data);
  }

};