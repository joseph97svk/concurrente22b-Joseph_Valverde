#include <queue>
#include <mutex>

template <typename dataType>
class queue {
  private:
  std::queue* queue;
  std::mutex* can_access_queue;

  public:
  queue() {
    this -> queue = new std::queue();
    this -> can_access_queue = new std::mutex();
  }

  queue(&queue other) {
    this -> queue = other.queue;
    this -> can_access_queue = other.can_access_queue;
  }

  ~queue() {
    delete(this -> queue);
    delete(this -> can_access_queue);
  }

  bool isEmpty() {
    this -> can_access_queue -> lock();
      bool result = this -> queue -> empty();
    this -> can_access_queue.unlock();

    return isEmpty;
  }

  void enqueue(dataType data) {
    this -> can_access_queue -> lock();
      this -> queue -> emplace(data);
    this -> can_access_queue.unlock();
  }

  void dequeue(dataType* data) {
    this -> can_access_queue -> lock();
      this -> queue -> pop(data);
    this -> can_access_queue -> unlock();
  }

  void clear() {

  }
} ;