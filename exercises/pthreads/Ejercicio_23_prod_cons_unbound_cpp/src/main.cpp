#include <cstdlib>

#include "common.hpp"
#include "simulation.hpp"

int main (int argc, char* argv[]) {
  simulation* simulation = new class simulation();

  simulation -> run(argc, argv);

}