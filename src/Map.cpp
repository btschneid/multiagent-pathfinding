#include "Map.h"

// Define the map folder path
const std::string Map::MAP_FOLDER_PATH = "../tests/maps/";

Map::Map(const std::string& _map_name) : map_name(_map_name) {
  std::string full_path = MAP_FOLDER_PATH + _map_name + ".map";
  if (!InitializeMap(full_path)) {
    throw std::runtime_error("Failed to initialize map from file: " + _map_name);
  }
}

bool Map::InitializeMap(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open map file: " << file_path << std::endl;
    return false;
  }

  std::string line;
  
  // Get map type
  std::getline(file, line);
  std::istringstream type_stream(line);
  std::string type_label;
  type_stream >> type_label >> type_label;
  movement_type = MovementType::OCTILE;

  // Get map height
  std::getline(file, line);
  std::istringstream height_stream(line);
  std::string height_label;
  height_stream >> height_label >> map_height;
  if (height_label != "height" || map_height <= 0) {
    std::cerr << "Error: Invalid height in map file." << std::endl;
    return false;
  }

  // Get map width
  std::getline(file, line);
  std::istringstream width_stream(line);
  std::string width_label;
  width_stream >> width_label >> map_width;
  if (width_label != "width" || map_width <= 0) {
    std::cerr << "Error: Invalid width in map file." << std::endl;
    return false;
  }

  // Read "map" line (fourth line)
  std::getline(file, line);
  if (line != "map") {
    std::cerr << "Error: Expected 'map' line." << std::endl;
    return false;
  }

  // Read grid data
  grid.resize(map_height, std::vector<std::shared_ptr<Cell>>(map_width));
  for (int y = 0; y < map_height; ++y) {
    std::getline(file, line);
    if (line.length() != static_cast<size_t>(map_width)) {
      std::cerr << "Error: Map width mismatch at line " << y + 1 << std::endl;
      return false;
    }
    for (int x = 0; x < map_width; ++x) {
      grid[y][x] = std::make_shared<Cell>(line[x]); // Initialize each cell with its icon
    }
  }

  return true;
}

std::shared_ptr<Cell> Map::GetCell(int x, int y) const {
  if (IsInBounds(x, y)) {
    return grid[x][y];
  }
  return nullptr;
}

void Map::PrintMap() const {
  for (const auto& row : grid) {
    for (const auto& cell : row) {
      std::cout << cell->icon;
    }
    std::cout << std::endl;
  }
}

bool Map::IsObstacle(int x, int y) const {
  return grid[x][y]->IsObstacle();  // Return true if the cell is an obstacle
}

bool Map::IsOccupied(int x, int y, int time) const {
  if (!IsInBounds(x, y)) {
    return false;
  }

  // Check if the cell is occupied at the given time
  return grid[y][x]->occupancy_map.find(time) != grid[y][x]->occupancy_map.end();
}

int Map::GetAgentAt(int x, int y, int time) const {
  if (!IsInBounds(x, y)) {
    return -1;  // Return -1 if the position is out of bounds
  }

  // Check if the cell has an agent occupying it at the given time
  auto it = grid[y][x]->occupancy_map.find(time);
  if (it != grid[y][x]->occupancy_map.end()) {
    return it->second;  // Return the agent ID at that time
  }
  return -1;
}

bool Map::IsInBounds(int x, int y) const {
  // Ensure the coordinates are within the map's bounds
  return x >= 0 && y >= 0 && x < map_width && y < map_height;
}