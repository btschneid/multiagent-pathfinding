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
#include <queue>
#include <unordered_set>
#include <deque>
#include <SFML/Graphics.hpp>

// Cell structure representing each cell in the map
struct Cell {
  int row;
  int col;
  char icon; // Icon of the map ('.', '@', etc)
  std::unordered_set<char> icons;
  std::unordered_map<int, std::deque<int>> occupancy_map; // Time -> queue of agent_ids (FIFO)

  // Constructor to initialize the cell
  Cell(int _r, int _c, char _icon = '.');

  // Check if the cell is an obstacle
  bool IsObstacle() const;

  // Check if the cell is occupied at a specific time
  bool IsOccupiedAtTime(int time) const;

  // Occupy the cell at a specific time with a given agent
  void Occupy(int time, int agent_id);

  // Free the cell at a specific time for an agent
  void Free(int time, int agent_id);
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
    bool IsObstacle(int row, int col) const;
    bool IsOccupied(int row, int col, int time) const;
    int GetAgentAt(int row, int col, int time) const;
    bool IsInBounds(int row, int col) const;
    std::shared_ptr<Cell> GetCell(int row, int col) const;

    // Pathfinding and validation
    std::vector<std::pair<int, int>> GetNeighbors(int x, int y) const;

    // Movement cost and heuristic
    double GetMovementCost(int row1, int col1, int row2, int col2) const;
    double Heuristic(int row1, int col1, int row2, int col2) const;

    // Visualization
    void PrintMap() const;
    void VisualizeMap() const;

  private:
    std::vector<std::vector<std::shared_ptr<Cell>>> grid; // 2D grid of cells
    std::string map_name;                // Name of the map
    int map_height;                      // Height of the map
    int map_width;                       // Width of the map
    MovementType movement_type;          // Movement type (Manhattan or Octile)
};

#endif // MAP_H