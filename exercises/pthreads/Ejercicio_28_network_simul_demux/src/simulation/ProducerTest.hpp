/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#ifndef PRODUCERTEST_HPP
#define PRODUCERTEST_HPP

#include "NetworkMessage.hpp"
#include "../prodcons/Producer.hpp"

class sharedData;

/**
 * @brief A productor class example
 * Produces network messages and push them to the queue
 */
class ProducerTest : public Producer<NetworkMessage> {
  DISABLE_COPY(ProducerTest);

 protected:
  /// Number of packages to be produced
  size_t packageCount = 0;
  /// Delay of producer to create a package, negative for max random
  int productorDelay = 0;
  /// Number of consumer threads
  size_t consumerCount = 0;

  size_t packagesProduced = 0;

  sharedData* sharedPackageCount;

  size_t id;

 public:
  /// Constructor
  ProducerTest(size_t packageCount, int productorDelay,
  size_t consumerCount, sharedData* sharedPackageCount, size_t id = 0);
  /// Do the message production in its own execution thread
  int run() override;

  size_t getPackageCount();

  /// Creates a simulation message to be sent by the network
  NetworkMessage createMessage(size_t index) const;
};

#endif  // PRODUCERTEST_HPP
