#include <iostream>
#include <sstream>

#include "simulation.hpp"

void simulation::run (int argc, char* argv[]) {
  analyzeArguments(argc, argv);

  createConsumersProducers();
}

void simulation::analyzeArguments(int argc, char* argv[]) {
  this -> simulation_data = new class simulation_data();
  if (argc == 8) {
    std::istringstream argument1Buffer(argv[1]);
    argument1Buffer >> simulation_data -> unit_count;
    std::istringstream argument2Buffer(argv[2]);
    argument2Buffer >> simulation_data -> producer_count;
    std::istringstream argument3Buffer(argv[3]);
    argument3Buffer >> simulation_data -> consumer_count;
    std::istringstream argument4Buffer(argv[4]);
    argument4Buffer >> simulation_data -> producer_min_delay;
    std::istringstream argument5Buffer(argv[5]);
    argument4Buffer >> simulation_data -> producer_max_delay;
    std::istringstream argument6Buffer(argv[6]);
    argument4Buffer >> simulation_data -> consumer_min_delay;
    std::istringstream argument7Buffer(argv[7]);
    argument4Buffer >> simulation_data -> consumer_max_delay;
  } else {
  }
}

void simulation::createConsumersProducers() {

}

void createThreads () {

}

void simulation::joinThreads() {

}
