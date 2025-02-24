#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>

// Cell structure representing each cell in the map
struct Cell {
  char icon; // Icon of the map ('.', '@', etc)
  std::unordered_map<int, int> occupancy_map; // Time of occupancy -> agent_id who is occupying cell

  // Constructor to initialize the cell
  Cell(char _icon = '.') : icon(_icon) {}

  // Check if the cell is an obstacle
  bool IsObstacle() const {
    return icon != '.';
  }
};

// Enum for movement types
enum class MovementType {
    MANHATTAN, // 4-directional
    OCTILE     // 8-directional
};

class Map {
  public:
    // Constant for the map folder path
    static const std::string MAP_FOLDER_PATH;

    // Constructor
    Map(const std::string& _map_name);
    bool InitializeMap(const std::string& file_path);

    // Map querying
    bool IsObstacle(int x, int y) const;
    bool IsOccupied(int x, int y, int time) const;
    int GetAgentAt(int x, int y, int time) const;
    bool IsInBounds(int x, int y) const;
    std::shared_ptr<Cell> GetCell(int x, int y) const;

    // Map modification
    void SetObstacle(int x, int y, bool is_obstacle);
    void AddAgent(int x, int y, int agent_id, int time);
    void RemoveAgent(int x, int y, int time);

    // Pathfinding and validation
    std::vector<std::pair<int, int>> GetNeighbors(int x, int y) const;

    // Movement cost and heuristic
    double GetMovementCost(int x1, int y1, int x2, int y2) const;
    double Heuristic(int x1, int y1, int x2, int y2) const;

    // Visualization
    void PrintMap() const;

  private:
    std::vector<std::vector<std::shared_ptr<Cell>>> grid; // 2D grid of cells
    std::string map_name;                // Name of the map
    int map_height;                      // Height of the map
    int map_width;                       // Width of the map
    MovementType movement_type;          // Movement type (Manhattan or Octile)
};

#endif // MAP_H