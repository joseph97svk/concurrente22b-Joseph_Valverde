/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <mutex>
#include <queue>

#include "../common/common.hpp"
#include "Semaphore.hpp"

/**
 * @brief A thread-safe generic queue for consumer-producer pattern.
 *
 * @remark None of the methods of this class can be const because all
 * methods require lock the mutex to avoid race-conditions
 */
template <typename DataType>
class Queue {
  DISABLE_COPY(Queue);

 protected:
  /// All read or write operations are mutually exclusive
  std::mutex mutex;

  Semaphore canProduce;
  /// Indicates if there are consumable elements in the queue
  Semaphore* canConsume;
  /// Contains the actual data shared between producer and consumer
  std::queue<DataType> queue;

  bool ownsCanConsume = false;

 public:
  /// Constructor
  explicit Queue(size_t queueCapacity = MAXQUEUESIZE,
  Semaphore* canConsume = nullptr)
    :
    canProduce(queueCapacity),
    canConsume(canConsume) {
    if (this->canConsume == nullptr) {
      this->canConsume = new Semaphore(0);
      this->ownsCanConsume = true;
    }
  }

  /// Destructor
  ~Queue() {
    if (this->ownsCanConsume) {
      delete(this->canConsume);
    }
    // TODO(jhc): clear()?
  }

  /// Produces an element that is pushed in the queue
  /// The semaphore is increased to wait potential consumers
  void push(const DataType& data) {
    this->canProduce.wait();
    this->mutex.lock();
    this->queue.push(data);
    this->mutex.unlock();
    this->canConsume->signal();
  }

  /// Consumes the next available element. If the queue is empty, blocks the
  /// calling thread until an element is produced and enqueue
  /// @return A copy of the element that was removed from the queue
  DataType pop() {
    this->canConsume->wait();
    this->mutex.lock();
    DataType result = this->queue.front();
    this->queue.pop();
    this->mutex.unlock();
    this->canProduce.signal();
    return result;
  }
};

#endif  // QUEUE_HPP