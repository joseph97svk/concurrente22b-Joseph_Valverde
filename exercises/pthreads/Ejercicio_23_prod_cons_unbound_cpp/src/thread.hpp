#include <thread>

class thread  {
  protected:
  std::thread* thread;

  public:
  virtual void* process(void* data);
  virtual void initThread(void* data);
  virtual ~thread() {}

  
};