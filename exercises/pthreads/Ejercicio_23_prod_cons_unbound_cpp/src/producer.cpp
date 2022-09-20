

#include "producer.hpp"

void* producer::process (void* data) {

}

void producer::initThread(void* data) {
    this->thread = new std::thread(process, data);
}