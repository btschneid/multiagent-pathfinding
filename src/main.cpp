#include "Manager.h"
#include <iostream>

int main() {
  const std::string MAP_NAME = "maze-32-32-4";
  const std::string SCENARIO_FOLDER = "maze-32-32-2";
  const std::string SCENARIO_NAME = "maze-32-32-2-even-1";
  const int AGENTS_COUNT = 10;

  Manager manager;

  try {
    // Initialize the map with the file name
    manager.InitializeMap(MAP_NAME);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  //manager.PrintMap();

  try {
    // Initialize the scenario
    manager.InitializeScenario(MAP_NAME, SCENARIO_FOLDER, SCENARIO_NAME, AGENTS_COUNT);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}