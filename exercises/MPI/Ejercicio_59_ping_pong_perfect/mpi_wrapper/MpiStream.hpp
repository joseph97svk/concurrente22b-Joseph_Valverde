#pragma once

#ifndef MPISTREAM_HPP
#define MPISTREAM_HPP

#include "Mpi.hpp"

class MpiStream {
  Mpi& mpi;
  int processNumber = -1;

 public:
  MpiStream(Mpi& mpi, int processNumber)
  : mpi(mpi)
  , processNumber(processNumber) {
  }

  template <typename dataType>
  MpiStream& operator << (dataType& data) {
    this->mpi.send(data, this->processNumber);
    
    return *this;
  }

  MpiStream& operator << (std::string& text) {
    this->mpi.send(text, this->processNumber);
    
    return *this;
  }

  template <typename dataType>
  MpiStream& operator >> (dataType& data) {
    this->mpi.receive(data, this->processNumber);
    return *this;
  }

} ;

#endif