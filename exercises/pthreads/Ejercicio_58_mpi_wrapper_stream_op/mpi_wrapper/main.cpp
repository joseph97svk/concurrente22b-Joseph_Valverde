#include <iostream>
#include <sstream>

#include "Mpi.hpp"

const size_t MESSAGE_CAPACITY = 512;

void greet(Mpi& mpi, int processNumber, int processCount,
    std::string& processHostname);

int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);
    const int rank = mpi.getProcessNumber();
    const int count = mpi.getProcessCount();
    std::string hostName = mpi.getHostname();

    greet(mpi, rank, count, hostName);
  
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
  } 
}

void greet(Mpi& mpi, int processNumber, int processCount,
    std::string& processHostname) {
  (void) processHostname;

  if (processNumber != 0) {
    const std::string& message = "Hello from " + std::to_string(processNumber);
    mpi[0] << message;
    
  } else {
    std::string text;
    for (int source = 1; source < processCount; ++source) {
      mpi[source] >> text;

      std::cout << source << " says: " << text << std::endl;
    }
  }

}