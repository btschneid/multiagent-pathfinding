#include "Manager.h"

// Define the map folder path
const std::string Manager::SCENARIO_FOLDER_PATH = "../tests/scenarios/";

// Constructor
Manager::Manager() : map(nullptr), agents(), next_agent_id(0) {}

// Initialize the map
void Manager::InitializeMap(const std::string& _map_name) {
  try {
    map = std::make_shared<Map>(_map_name);
  } catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    throw;
  }
}

// Print the map to console
void Manager::PrintMap() const {
  if (map) {
    map->PrintMap();
  } else {
    std::cerr << "Error: Map is not initialized." << std::endl;
  }
}

// Visualize the map in a window
void Manager::VisualizeMap() const {
  if (map) {
    map->VisualizeMap();
  } else {
    std::cerr << "Error: Map is not initialized." << std::endl;
  }
}

// Visualize the map in a window with a specific title
void Manager::VisualizeMapWithTitle(const std::string& title) const {
  if (map) {
    map->VisualizeMapWithTitle(title);
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
    while (next_agent_id < agents_count && std::getline(scenario_file, line)) {
        std::istringstream line_stream(line);
        int bucket, height, width, start_x, start_y, goal_x, goal_y;
        std::string map_name;
        double optimal_distance;

        // Parse the line
        line_stream >> bucket >> map_name >> height >> width >> start_x >> start_y >> goal_x >> goal_y >> optimal_distance;

        agents.push_back(std::make_shared<Agent>(next_agent_id, start_x, start_y, goal_x, goal_y, optimal_distance, map));

        // Print or store the agent data (for now, just print)
        std::cout << std::fixed << std::setprecision(10)
                  << "Agent " << next_agent_id + 1 << ":\n"
                  << "  Start: (" << start_x << ", " << start_y << ")\n"
                  << "  Goal: (" << goal_x << ", " << goal_y << ")\n"
                  << "  Optimal Distance: " << optimal_distance << "\n"
                  << std::endl;

        // Increment the number of agents loaded
        next_agent_id++;
    }

    if (next_agent_id < agents_count) {
        throw std::runtime_error("Not enough agents in the scenario file: " + folder_path);
    }

}

void Manager::StartPathfind() {
  CBS cbs(map, agents);
  cbs.PathFind();

  // Loop through all agents
  for (size_t i = 0; i < agents.size(); ++i) {
    auto& agent = agents[i];
    
    // Determine the icon for this agent
    char icon;
    if (i < 26) {
      // A-Z for first 26 agents
      icon = 'A' + static_cast<char>(i);
    }
    
    // Loop through the agent's path and set the icon
    for (auto& cell : agent->GetPath()) {
      if (cell) {
        cell->icon = icon;
      }
    }
  }
}

