#include "consumer.hpp"

void* consumer::process (void* data) {

}

void consumer::initThread(void* data) {
    this -> thread = new class std::thread(process, data);
}