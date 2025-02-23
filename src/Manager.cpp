#include "Manager.h"

// Define the map folder path
const std::string Manager::SCENARIO_FOLDER_PATH = "../tests/scenarios/";

// Constructor
Manager::Manager() {}

// Initialize the map
void Manager::InitializeMap(const std::string& _map_name) {
  try {
    map = std::make_unique<Map>(_map_name);
  } catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    throw;
  }
}

// Print the map
void Manager::PrintMap() const {
  if (map) {
    map->PrintMap();
  } else {
    std::cerr << "Error: Map is not initialized." << std::endl;
  }
}

// Initialize the Agent(s)
void Manager::InitializeScenario(const std::string& _map_name, const std::string& _scenario_folder_name, const std::string& _scenario_name, const int agents_count) {
    // Construct the full path to the scenario file
    std::string folder_path = SCENARIO_FOLDER_PATH + _scenario_folder_name + "/" + _scenario_name + ".scen";

    // Open the scenario file
    std::ifstream scenario_file(folder_path);
    if (!scenario_file.is_open()) {
        throw std::runtime_error("Failed to open scenario file: " + folder_path);
    }

    // Skip the header line
    std::string line;
    std::getline(scenario_file, line);

    // Read agents_count number of agents
    int agents_loaded = 0;
    while (agents_loaded < agents_count && std::getline(scenario_file, line)) {
        std::istringstream line_stream(line);
        int id, height, width, start_x, start_y, goal_x, goal_y;
        std::string map_name;
        double optimal_distance;

        // Parse the line
        line_stream >> id >> map_name >> height >> width >> start_x >> start_y >> goal_x >> goal_y >> optimal_distance;

        // Print or store the agent data (for now, just print)
        std::cout << std::fixed << std::setprecision(10)
                  << "Agent " << agents_loaded + 1 << ":\n"
                  << "  Start: (" << start_x << ", " << start_y << ")\n"
                  << "  Goal: (" << goal_x << ", " << goal_y << ")\n"
                  << "  Optimal Distance: " << optimal_distance << "\n"
                  << std::endl;

        // Increment the number of agents loaded
        agents_loaded++;
    }

    if (agents_loaded < agents_count) {
        throw std::runtime_error("Not enough agents in the scenario file: " + folder_path);
    }

    std::cout << "Successfully loaded " << agents_loaded << " agents from scenario file: " << folder_path << std::endl;
}