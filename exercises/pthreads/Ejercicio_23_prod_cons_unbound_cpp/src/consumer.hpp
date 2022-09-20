#include "thread.hpp"

class consumer: public thread {
    public:
    void* process (void* data);

    void initThread(void* data);
};