// Copyright 2022 Joseph Valverde <joseph.valverdekong@ucr.ac.cr>
#include <random>

#include "AssemblerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"


/**
 * @brief Generates random number between 0 and 100
 * 
 * @return double 
 */
double generateRandomNumber();

// Constructor
AssemblerTest::AssemblerTest(
int consumerDelay,
size_t packageCount, int productorDelay, size_t consumerCount,
double packageLossPercentage):
ConsumerTest(consumerDelay),
ProducerTest(packageCount, productorDelay, consumerCount, nullptr),
packageLossPercentage(packageLossPercentage) {
}

int AssemblerTest::run() {
  this->consumeForever();

  Log::append(Log::INFO, "Assembler:",
  std::to_string(this->receivedMessages) + " packages received");

  Log::append(Log::INFO, "Assembler:",
  std::to_string(this->packageCount) + " packages sent back");

  Log::append(Log::INFO, "Assembler:",
  std::to_string(this->packagesLost) + " packages lost");

  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  (this->receivedMessages)++;
  // if within percentage send back to queue
  if (generateRandomNumber() >= this->packageLossPercentage) {
    this->produce(data);
    (this->packageCount)++;
  } else {
    // else: do nothing, gets lost
    (this->packagesLost)++;
  }
}

double generateRandomNumber() {
  std::random_device random;
  std::mt19937 generator(random());
  std::uniform_real_distribution<> distribution(1, 100);

  return distribution(generator);
}
