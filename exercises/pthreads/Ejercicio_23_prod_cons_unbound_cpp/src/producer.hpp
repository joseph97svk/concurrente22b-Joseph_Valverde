#include "thread.hpp"

class producer: public thread {
    public:
    void* process (void* data);

    void initThread(void* data);
};