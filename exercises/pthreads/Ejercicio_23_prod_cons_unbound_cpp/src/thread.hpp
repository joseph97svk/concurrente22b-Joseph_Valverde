// Copyright [2022] <Joseph Valverde>
#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>

template <typename dataType>
class thread  {
 protected:
  std::thread* myThread;

 public:
  thread():
  myThread(nullptr) {
  }

  virtual void process(dataType data) = 0;

  void initThread(dataType data) {
    myThread = new class std::thread(&thread::process, this, data);
  }

  ~thread() {
    if (this->myThread == nullptr) {
      return;
    }

    if (!this->myThread->joinable()) {
      return;
    }
    this->myThread->join();
    delete this->myThread;
    this->myThread = nullptr;
  }

  void join() {
    this->myThread->join();
    delete this->myThread;
    this->myThread = nullptr;
  }
};

#endif
