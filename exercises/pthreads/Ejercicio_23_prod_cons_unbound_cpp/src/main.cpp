// Copyright [2022] <Joseph Valverde>
#include <cstdlib>

#include "simulation.hpp"

int main(int argc, char* argv[]) {
  simulation* simulation = new class simulation();

  simulation -> run(argc, argv);

  delete(simulation);
}
