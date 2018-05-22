#include "simulator.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
    return 1;
  }
  Simulator simulator(argv[1]);
  simulator.Run();
  return 0;
}
