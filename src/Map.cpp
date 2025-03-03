#include "Map.h"

// Define the map folder path
const std::string Map::MAP_FOLDER_PATH = "../tests/maps/";

Cell::Cell(int _r, int _c, char _icon) : row(_r), col(_c), icon(_icon) {}

// Check if the cell is an obstacle
bool Cell::IsObstacle() const {
  return icon == '@';
}

// Check if the cell is occupied at a specific time
bool Cell::IsOccupiedAtTime(int time) const {
  return occupancy_map.find(time) != occupancy_map.end() && !occupancy_map.at(time).empty();
}

// Occupy the cell at a specific time with a given agent
void Cell::Occupy(int time, int agent_id) {
  occupancy_map[time].push_back(agent_id);
}

// Free the cell at a specific time for an agent
void Cell::Free(int time, int agent_id) {
  if (occupancy_map.find(time) != occupancy_map.end()) {
    std::deque<int>& agents_at_time = occupancy_map[time];
    std::deque<int> temp_queue;
    
    // Remove the specific agent (FIFO behavior)
    bool found = false;
    while (!agents_at_time.empty()) {
      int current_agent = agents_at_time.front();
      agents_at_time.pop_front();
      
      if (current_agent == agent_id && !found) {
        found = true; // Skip the agent being freed
      } else {
        temp_queue.push_back(current_agent); // Keep the others in the queue
      }
    }

    // Restore the modified queue
    occupancy_map[time] = temp_queue;

    // Clean up if no agents are left at this time
    if (occupancy_map[time].empty()) {
      occupancy_map.erase(time);
    }
  }
}

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
  type_stream >> type_label >> type_label;  // Read the label twice if needed

  // Determine movement type based on the type_label
  if (type_label == "octile") {
    movement_type = MovementType::OCTILE;
  } else if (type_label == "manhattan") {
    movement_type = MovementType::MANHATTAN;
  } else {
    // Handle the case for an unknown type, maybe default or throw an error
    std::cerr << "Unknown movement type: " << type_label << std::endl;
    movement_type = MovementType::MANHATTAN; // Default or error handling
  }


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
  for (int row = 0; row < map_height; ++row) {
    std::getline(file, line);
    if (line.length() != static_cast<size_t>(map_width)) {
      std::cerr << "Error: Map width mismatch at line " << row + 1 << std::endl;
      return false;
    }
    for (int col = 0; col < map_width; ++col) {
      grid[row][col] = std::make_shared<Cell>(row, col, line[col]); // Initialize each cell with its icon
    }
  }

  return true;
}

std::shared_ptr<Cell> Map::GetCell(int row, int col) const {
  if (IsInBounds(row, col)) {
    return grid[row][col];
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

bool Map::IsObstacle(int row, int col) const {
  return grid[row][col]->IsObstacle();  // Return true if the cell is an obstacle
}

bool Map::IsOccupied(int row, int col, int time) const {
  if (!IsInBounds(row, col)) {
    return false;
  }

  // Check if the cell is occupied at the given time
  return grid[row][col]->occupancy_map.find(time) != grid[row][col]->occupancy_map.end();
}

int Map::GetAgentAt(int row, int col, int time) const {
  if (!IsInBounds(row, col)) {
    return -1;  // Return -1 if the position is out of bounds
  }

  // Check if the cell has an agent occupying it at the given time
  auto it = grid[row][col]->occupancy_map.find(time);
  if (it != grid[row][col]->occupancy_map.end()) {
    return it->second.front();  // Return the agent ID at that time
  }
  return -1;
}

bool Map::IsInBounds(int row, int col) const {
  // Ensure the coordinates are within the map's bounds
  return row >= 0 && col >= 0 && row < map_height && col < map_width;
}

// Movement cost between two cells
double Map::GetMovementCost(int row1, int col1, int row2, int col2) const {
  if (!IsInBounds(row1, col1) || !IsInBounds(row2, col2)) {
    return std::numeric_limits<double>::max(); // Large cost if out of bounds
  }
  
  if (IsObstacle(row2, col2)) {
    return std::numeric_limits<double>::max(); // Infinite cost for obstacles
  }

  // Manhattan (4-way) movement
  if (movement_type == MovementType::MANHATTAN) {
    return 1.0; // Uniform cost for horizontal/vertical moves
  }

  // Octile (8-way) movement
  if (movement_type == MovementType::OCTILE) {
    int drow = std::abs(row2 - row1);
    int dcol = std::abs(col2 - col1);
    if (drow + dcol == 1) return 1.0;         // Horizontal or vertical move
    if (drow == 1 && dcol == 1) return sqrt(2); // Diagonal move (sqrt(2))
  }

  return std::numeric_limits<double>::max(); // Default case (shouldn't be reached)
}

// Heuristic function for A* (estimates cost from (row1, col1) to (row2, col2))
double Map::Heuristic(int row1, int col1, int row2, int col2) const {
  int drow = std::abs(row2 - row1);
  int dcol = std::abs(col2 - col1);

  if (movement_type == MovementType::MANHATTAN) {
    return drow + dcol; // Manhattan distance
  }
  
  if (movement_type == MovementType::OCTILE) {
    return std::max(drow, dcol) + (std::sqrt(2) - 1) * std::min(drow, dcol); // Octile distance
  }

  return 0.0; // Default case
}

// Returns a list of valid neighboring cells for (row, col)
std::vector<std::pair<int, int>> Map::GetNeighbors(int row, int col) const {
  std::vector<std::pair<int, int>> neighbors;
  std::vector<std::pair<int, int>> directions;

  // Define movement directions based on movement type
  if (movement_type == MovementType::MANHATTAN) {
    directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}; // Down, Right, Up, Left
  } else if (movement_type == MovementType::OCTILE) {
    directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1},  // Cardinal directions
                  {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}; // Diagonal directions
  }

  // Iterate through potential neighbor positions
  for (const auto& dir : directions) {
    int nrow = row + dir.first;
    int ncol = col + dir.second;

    // Check if the neighbor is within bounds and not an obstacle
    if (IsInBounds(nrow, ncol) && !IsObstacle(nrow, ncol)) {
      neighbors.emplace_back(nrow, ncol);
    }
  }

  return neighbors;
}

void Map::VisualizeMap() const {
  const int cellSize = 20; // Size of each cell in pixels
  sf::RenderWindow window(sf::VideoMode(map_width * cellSize, map_height * cellSize), "Map Visualization");

  std::unordered_map<char, sf::Color> colorMap = {
      {'.', sf::Color::White},
      {'@', sf::Color::Black},
      {'A', sf::Color::Red},
      {'B', sf::Color::Blue},
      {'C', sf::Color::Green},
      {'D', sf::Color::Yellow},
      {'E', sf::Color::Magenta},
      {'F', sf::Color::Cyan} // Add more mappings as needed
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    for (int row = 0; row < map_height; ++row) {
      for (int col = 0; col < map_width; ++col) {
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
        cellShape.setPosition(col * cellSize, row * cellSize);

        const std::unordered_set<char>& icons = grid[row][col]->icons; // Assuming each cell has a set of icons
        char icon = grid[row][col]->icon;
        
        if (icon == '.' || icon == '@') {
          cellShape.setFillColor(colorMap[icon]);
        }
        else if (!icons.empty()) {
          int totalR = 0, totalG = 0, totalB = 0;
          for (char icon : icons) {
            if (colorMap.find(icon) != colorMap.end()) {
              sf::Color color = colorMap[icon];
              totalR += color.r;
              totalG += color.g;
              totalB += color.b;
            }
          }
          int count = icons.size();
          sf::Color blendedColor(totalR / count, totalG / count, totalB / count);
          cellShape.setFillColor(blendedColor);
        } else {
          cellShape.setFillColor(sf::Color::Green); // Default color for unknown characters
        }

        cellShape.setOutlineColor(sf::Color::Black);
        cellShape.setOutlineThickness(1);
        window.draw(cellShape);
      }
    }

    window.display();
  }
}