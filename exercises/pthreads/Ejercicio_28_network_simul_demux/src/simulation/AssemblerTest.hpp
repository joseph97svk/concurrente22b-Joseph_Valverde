#include "ConsumerTest.hpp"
#include "ProducerTest.hpp"
#include "NetworkMessage.hpp"

class AssemblerTest:
public ConsumerTest, public ProducerTest {
  DISABLE_COPY(AssemblerTest);
 protected:
  /// @brief percentage of packages that will be lost
  double packageLossPercentage;
  
  /// @brief  amount of packages lost
  int64_t packagesLost = 0;

 public: 
  AssemblerTest(
  int consumerDelay = 0,
  size_t packageCount = 0, int productorDelay = 0,
  size_t consumerCount = 0,
  double packageLossPercentage = -1);

  ~AssemblerTest() = default;

  int run() override;

  void consume(NetworkMessage data) override;
};