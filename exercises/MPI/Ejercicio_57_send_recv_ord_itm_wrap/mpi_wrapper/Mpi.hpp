#ifndef MPI_HPP
#define MPI_HPP

#include <string>
#include <stdexcept>
#include <mpi.h>
#include <MpiError.hpp>
#include <vector>
#include <string>

class Mpi {

  int processNumber = -1;
  int processCount = -1;
  std::string processHostname;

 public:
  /**
   * @brief Construct a new Mpi object
   * 
   * @param argc argument count
   * @param argv argument vector
   */
  Mpi(int& argc, char**& argv) {
    // initialize MPI environment
    if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
      // get rank
      if (MPI_Comm_rank(MPI_COMM_WORLD, &(this->processNumber))
      != MPI_SUCCESS) {
        throw MpiError("Could not get process rank");
      }

      // get size
      if (MPI_Comm_size(MPI_COMM_WORLD, &(this->processCount))
      != MPI_SUCCESS) {
        throw MpiError("Could not get process amount", *this);
      }
      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      
      // get host name
      if (MPI_Get_processor_name(process_hostname, &hostname_length)
      == MPI_SUCCESS) {
        this->processHostname = process_hostname;
      } else {
        throw MpiError("Could not get process name", *this);
      }
    } else {
      throw MpiError("Could not initialize MPI enviroment");
    }
  }

  /**
   * @brief Destroy the Mpi object
   * 
   */
  ~Mpi() {
    MPI_Finalize();
  }

  /**
   * @brief Get the Process Count object
   * 
   * @return int amount of processess in MPI world
   */
  inline int getProcessCount() const {
    return this->processCount;
  }

  /**
   * @brief Get the Process Number object
   * 
   * @return int rank of process
   */
  inline int getProcessNumber() const {
    return this->processNumber;
  }

  /**
   * @brief Get the hostname 
   * 
   * @return const std::string& name of the host
   */
  inline const std::string& getHostname() const {
    return this->processHostname;
  }

  template <typename dataType>
  void send(dataType* value, const size_t count,
      const int toProcess, const int tag = 0) {
    if (MPI_Send(value, count, map(value), toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS) {
      throw MpiError("Could not send data");
    }
  }

  template <typename dataType>
  void send(std::vector<dataType>& values,
      const int toProcess, const int tag = 0) {
    if (MPI_Send(values.data(), values.size(), map(values[0]),
        toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS) {
      throw MpiError("Could not send data");
    }
  }

  void send(const std::string& text, const int toProcess, const int tag = 0) {
    if (MPI_Send(&text[0], text.size(), map(text[0]),
        toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS) {
      throw MpiError("Could not send data");
    }
  }

  template <typename dataType>
  void send(dataType& value, const int toProcess, const int tag = 0) {
    if (MPI_Send(&value, 1, map(value), toProcess, tag, MPI_COMM_WORLD)
        != MPI_SUCCESS) {
      throw MpiError("Could not send data");
    }
  }

  template <typename dataType>
  void receive(dataType* values, const size_t capacity, int fromProcess,
      const int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values, capacity, map(values[0]), fromProcess,
        tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
        != MPI_SUCCESS) {
      throw MpiError("Could not receive data");
    }
  }

  template <typename dataType>
  void receive(std::vector<dataType>& values, const size_t capacity, int fromProcess,
      const int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values.data(), capacity, map(values[0]), fromProcess,
        tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
        != MPI_SUCCESS) {
      throw MpiError("Could not receive data");
    }
  }

  void receive(std::string& text, const size_t capacity, int fromProcess,
      const int tag = MPI_ANY_TAG) {
    if (MPI_Recv(&text[0], capacity, map(text[0]), fromProcess,
        tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
        != MPI_SUCCESS) {
      throw MpiError("Could not receive data");
    }
  }

  template <typename dataType>
  void receive(dataType& value, int fromProcess, const int tag = MPI_ANY_TAG) {
    if (MPI_Recv(&value, 1, map(value), fromProcess,
        tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
        != MPI_SUCCESS) {
      throw MpiError("Could not receive data");
    }
  }

 private:
  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
  static inline MPI_Datatype map(char) { return MPI_CHAR; }
  static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; }
  static inline MPI_Datatype map(short) { return MPI_SHORT; }
  static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; }
  static inline MPI_Datatype map(int) { return MPI_INT; }
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
  static inline MPI_Datatype map(long) { return MPI_LONG; }
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
  static inline MPI_Datatype map(unsigned long long) { return MPI_UNSIGNED_LONG_LONG; }
  static inline MPI_Datatype map(float) { return MPI_FLOAT; }
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }
};

#endif