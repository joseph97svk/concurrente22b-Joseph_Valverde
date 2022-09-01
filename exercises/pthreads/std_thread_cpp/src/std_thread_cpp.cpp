#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <memory>

struct sharedData {
    uint64_t threadCount;

    sharedData (uint64_t threadCount = 0) :
    threadCount(threadCount) {}
};

struct privateData {
    uint64_t threadNumber;  // rank
    std::shared_ptr<sharedData> dataShared;

    privateData (uint64_t threadNumber = 0, 
    std::shared_ptr<sharedData> dataShared = NULL): 
    threadNumber (threadNumber), 
    dataShared(dataShared) {}

    privateData(privateData& other):
    threadNumber(other.threadNumber),
    dataShared(other.dataShared) {}
}; 


void greet(privateData* privateData);
int create_threads(std::shared_ptr<sharedData> sharedData);

int main (int argc, char* argv[]) {
    uint64_t threadCount = std::thread::hardware_concurrency();

    if (argc == 2) {
        std::istringstream argumentBuffer(argv[1]);
        argumentBuffer >> threadCount;
        
    } else if (argc > 2) {
        std::cerr << "Error: invalid input" << std::endl;
        return EXIT_FAILURE;
    }   

    std::shared_ptr<sharedData> sharedData(new struct sharedData(threadCount));

    int64_t error = create_threads(sharedData);

    std::chrono::high_resolution_clock::now();

    std::cout << "Execution time " << std::endl;

    return error;
}

int create_threads(std::shared_ptr<sharedData> sharedData) {
    std::vector <std::thread> threads;
    threads.reserve(sharedData -> threadCount);
    std::vector <privateData*> privateData;
    privateData.reserve(sharedData -> threadCount);

    for (uint64_t thread_number = 0; thread_number < sharedData -> threadCount;
    ++thread_number) {
        privateData.emplace_back(new struct privateData(thread_number, sharedData));
        threads.emplace_back(std::thread(greet, privateData[thread_number]));
    }

    std::cout << "Hello from main thread" << std::endl;

    for (uint64_t thread_number = 0; thread_number < sharedData -> threadCount;
    ++thread_number) {
        threads[thread_number].join();
    }
    return EXIT_SUCCESS;
}

void greet(privateData* privateData) {
    std::cout << "Hello from secondary thread " << privateData -> threadNumber << 
    " of " << privateData -> dataShared -> threadCount << std::endl;
}