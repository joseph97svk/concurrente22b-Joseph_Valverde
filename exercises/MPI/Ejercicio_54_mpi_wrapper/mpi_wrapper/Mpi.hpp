#include <string>
#include <stdexcept>
#include <mpi.h>

template <typename dataType = int>
class Mpi {

  int processNumber = -1;
  int processCount = -1;
  int rank = -1;
  int totalProcessAmount = -1;
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
        throw std::runtime_error("Could not get process rank");
      }

      // get size
      if (MPI_Comm_size(MPI_COMM_WORLD, &(this->processCount))
      != MPI_SUCCESS) {
        throw std::runtime_error("Could not get process amount");
      }
      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      
      // get host name
      if (MPI_Get_processor_name(process_hostname, &hostname_length)
      == MPI_SUCCESS) {
        this->processHostname = process_hostname;
      } else {
        throw std::runtime_error("Could not get process name");
      }
    } else {
      throw std::runtime_error("Could not initialize MPI enviroment");
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
};