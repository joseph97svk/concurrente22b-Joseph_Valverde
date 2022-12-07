#include "Mpi.hpp"
#include <iostream>
#include <sstream>

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
    /*
    if (rank > 0) {
      mpi.receive(can_print, previous);
    }
    std::cout << "Hello from main thread of process " << rank << " of "
      << count << " on " << mpi.getHostname() << std::endl;
    mpi.send(can_print, next); */
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
  } 
}

void greet(Mpi& mpi, int processNumber, int processCount,
    std::string& processHostname) {
  std::stringstream messageBuffer;
  messageBuffer << "Hello from main thread of process " << processNumber
      << " of " << processCount << " on " << processHostname;

  if (processNumber != 0) {
    const std::string& message = messageBuffer.str();
    size_t messageSize =  message.size();

    mpi.send(messageSize, 0);
    mpi.send(message, 0);
  } else {
    std::cout << processNumber << " said " << messageBuffer.str() << std::endl;
    std::string message(MESSAGE_CAPACITY, '\0');
    for (int source = 1; source < processCount; ++source) {
      size_t size = -1;

      mpi.receive(size, source);
      mpi.receive(message, size, source);

      std::cout << source << " sent " << message << std::endl;
    }
  }

}