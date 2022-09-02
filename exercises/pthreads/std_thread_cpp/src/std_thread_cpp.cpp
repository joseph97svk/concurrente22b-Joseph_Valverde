#include <iostream>
#include <thread>
#include <vector>
#include <sstream>
#include <memory>
#include <stdexcept>

enum exceptionError {
    exitSuccessError,
    invalidInputError,
    sharedDataAllocationError, 
    privateDataAllocationError,
    threadNotJoinableError,
};

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

}; 


void greet(std::shared_ptr<privateData> privateData);
void create_threads(std::shared_ptr<sharedData> sharedData);

int main (int argc, char* argv[]) {
    // set default thread values
    uint64_t threadCount = std::thread::hardware_concurrency();

    try {
         // read the arguments 
        if (argc == 2) {
            std::istringstream argumentBuffer(argv[1]);
            argumentBuffer >> threadCount;
            
        } else if (argc > 2) {
            throw invalidInputError;
        }   
    } catch (enum exceptionError error) {
        if (error == invalidInputError) {
            std::cerr << "Error: invalid input, more than 2 arguments" << std::endl;
            return (int) error;
        }

    }
   
    // run program 
    try {
        // start shared data
        std::shared_ptr<sharedData> sharedData(new struct sharedData(threadCount));

        if (sharedData == nullptr) {
            throw sharedDataAllocationError;
        }

        // start timer
        auto start = std::chrono::high_resolution_clock::now();
        // create threads
        create_threads(sharedData);

        // stop timer and report timing
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = end - start;
        std::cout << "Execution time: " << elapsedTime.count()  << "s" << std::endl;
    } catch (enum exceptionError error) {
        switch (error) {
        case sharedDataAllocationError:
            std::cerr << "Error: shared thread data could not be allocated" << std::endl;
            break;

        case privateDataAllocationError:
            std::cerr << "Error: private thread data could not be allocated" << std::endl;
            break;
        case threadNotJoinableError:
            std::cerr << "Error: A thread could not be joined or terminated" << std::endl;
            break;
        default:
            std::cerr << "Error: unexpected exception found" << std::endl;
            break;
        }

        return (int) error;
    }
    
    return EXIT_SUCCESS;
}

void create_threads(std::shared_ptr<sharedData> sharedData) {
    // create array for threads and allocate space
    std::vector <std::thread> threads;
    threads.reserve(sharedData -> threadCount);

    // create array for private Data and allocate space
    std::vector <std::shared_ptr<privateData>> privateData;
    privateData.reserve(sharedData -> threadCount);

    // for all threads
    for (uint64_t thread_number = 0; thread_number < sharedData -> threadCount;
    ++thread_number) {
        // add the private data to the array
        privateData.emplace_back(
            std::shared_ptr<struct privateData>(
                new struct privateData(thread_number, sharedData)));
        if (privateData[thread_number] == nullptr) {
            throw privateDataAllocationError;
        } 
        // add threads to the array
        threads.emplace_back(std::thread(greet, privateData[thread_number]));
    }

    // greet from the main thread
    std::cout << "Hello from main thread" << std::endl;

    // for all threads
    for (uint64_t thread_number = 0; thread_number < sharedData -> threadCount;
    ++thread_number) {
        // check if they can be joined
        if (!threads[thread_number].joinable()) {
            throw threadNotJoinableError;
        }
        // join the thread
        threads[thread_number].join();
    }
}

void greet(std::shared_ptr<privateData> privateData) {
    // greet
    std::cout << "Hello from secondary thread " << privateData -> threadNumber << 
    " of " << privateData -> dataShared -> threadCount << std::endl;
}