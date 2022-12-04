#ifndef MPIERROR_HPP
#define MPIERROR_HPP

#include <stdexcept>

class Mpi;

class MpiError : public std::runtime_error {
 public:
  /**
   * @brief Construct a new Mpi Error object
   * 
   * @param message 
   */
  explicit MpiError(const std::string& message);

  /**
   * @brief Construct a new Mpi Error object
   * 
   * @param message 
   * @param mpi 
   */
  MpiError(const std::string& message, const Mpi& mpi);

  /**
   * @brief Construct a new Mpi Error object
   * 
   * @param message 
   * @param mpi 
   * @param threadNumber 
   */
  MpiError(const std::string& message, const Mpi& mpi, const int threadNumber);
};

#endif