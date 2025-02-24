#include "Manager.h"
#include <iostream>

int main() {
  const std::string MAP_NAME = "maze-32-32-4";
  const std::string SCENARIO_FOLDER = "maze-32-32-2";
  const std::string SCENARIO_NAME = "maze-32-32-2-even-1";
  const int AGENTS_COUNT = 1;

  Manager manager;

  try {
    // Initialize the map with the file name
    manager.InitializeMap(MAP_NAME);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  manager.PrintMap();
  std::cout << "==========================================================\n";

  try {
    // Initialize the scenario
    manager.InitializeScenario(MAP_NAME, SCENARIO_FOLDER, SCENARIO_NAME, AGENTS_COUNT);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  manager.StartPathfind();

  manager.PrintMap();


  return 0;
}