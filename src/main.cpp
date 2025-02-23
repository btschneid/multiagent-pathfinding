#include "CBS.h"
#include "Map.h"
#include "Agent.h"
#include "AStar.h"
#include <iostream>

int main() {
  try {
    // Initialize the map with the file name
    Map map("maze-32-32-4");
    map.PrintMap(); // Print the map to verify
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}