#include "Manager.h"
#include <iostream>
#include <cstdlib>  // For std::stoi

int main(int argc, char* argv[]) {
  const std::string MAP_NAME = "maze-32-32-4";
  const std::string SCENARIO_FOLDER = "maze-32-32-2";
  const std::string SCENARIO_NAME = "maze-32-32-2-even-1";
  
  // Default to 1 agent if no argument is provided
  int AGENTS_COUNT = 1;

  // Check if an argument is provided for the number of agents
  if (argc > 1) {
    try {
      AGENTS_COUNT = std::stoi(argv[1]);  // Convert the argument to an integer
    } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid argument for agents count. Using default value 1.\n";
    } catch (const std::out_of_range& e) {
      std::cerr << "Argument out of range. Using default value 1.\n";
    }
  }

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
    // Initialize the scenario with the specified number of agents
    manager.InitializeScenario(MAP_NAME, SCENARIO_FOLDER, SCENARIO_NAME, AGENTS_COUNT);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  manager.StartPathfind();

  manager.PrintMap();

  return 0;
}
