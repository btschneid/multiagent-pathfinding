/**
 * @file Map.h
 * @brief Defines the Map class and related structures for grid-based pathfinding.
 * 
 * This file contains the core data structures and interfaces for representing
 * and manipulating a grid-based map for pathfinding algorithms. It includes
 * the Cell structure for individual grid cells and the Map class for managing
 * the overall grid.
 */

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

/**
 * @brief Represents a single cell in the grid map.
 * 
 * Each cell contains information about its position, visual representation,
 * and occupancy status at different time steps.
 */
struct Cell {
  int row;  ///< Row coordinate in the grid
  int col;  ///< Column coordinate in the grid
  char icon;  ///< Visual representation of the cell ('.', '@', etc)
  std::unordered_set<char> icons;  ///< Set of icons for cells with multiple agents
  std::unordered_map<int, std::deque<int>> occupancy_map;  ///< Time -> queue of agent_ids (FIFO)

  /**
   * @brief Constructs a new Cell object.
   * 
   * @param _r Row coordinate
   * @param _c Column coordinate
   * @param _icon Visual representation character
   */
  Cell(int _r, int _c, char _icon = '.');

  /**
   * @brief Checks if the cell is an obstacle.
   * 
   * @return true if the cell is an obstacle ('@'), false otherwise
   */
  bool IsObstacle() const;

  /**
   * @brief Checks if the cell is occupied at a specific time step.
   * 
   * @param time The time step to check
   * @return true if the cell is occupied at the given time
   */
  bool IsOccupiedAtTime(int time) const;

  /**
   * @brief Marks the cell as occupied by an agent at a specific time.
   * 
   * @param time The time step
   * @param agent_id The ID of the occupying agent
   */
  void Occupy(int time, int agent_id);

  /**
   * @brief Removes an agent's occupation from the cell at a specific time.
   * 
   * @param time The time step
   * @param agent_id The ID of the agent to remove
   */
  void Free(int time, int agent_id);
};

/**
 * @brief Defines the movement types supported by the map.
 */
enum class MovementType {
    MANHATTAN,  ///< 4-directional movement (up, down, left, right)
    OCTILE      ///< 8-directional movement (including diagonals)
};

/**
 * @brief Represents a grid-based map for pathfinding.
 * 
 * The Map class manages a 2D grid of cells and provides functionality for
 * pathfinding, collision detection, and visualization.
 */
class Map {
  public:
    /// Path to the folder containing map files
    static const std::string MAP_FOLDER_PATH;

    /**
     * @brief Constructs a new Map object from a map file.
     * 
     * @param _map_name Name of the map file (without extension)
     * @throw std::runtime_error if map initialization fails
     */
    Map(const std::string& _map_name);

    /**
     * @brief Initializes the map from a file.
     * 
     * @param file_path Path to the map file
     * @return true if initialization was successful
     */
    bool InitializeMap(const std::string& file_path);

    /**
     * @brief Checks if a cell is an obstacle.
     * 
     * @param row Row coordinate
     * @param col Column coordinate
     * @return true if the cell is an obstacle
     */
    bool IsObstacle(int row, int col) const;

    /**
     * @brief Checks if a cell is occupied at a specific time.
     * 
     * @param row Row coordinate
     * @param col Column coordinate
     * @param time Time step
     * @return true if the cell is occupied
     */
    bool IsOccupied(int row, int col, int time) const;

    /**
     * @brief Gets the ID of the agent occupying a cell at a specific time.
     * 
     * @param row Row coordinate
     * @param col Column coordinate
     * @param time Time step
     * @return Agent ID if occupied, -1 otherwise
     */
    int GetAgentAt(int row, int col, int time) const;

    /**
     * @brief Checks if coordinates are within map bounds.
     * 
     * @param row Row coordinate
     * @param col Column coordinate
     * @return true if coordinates are valid
     */
    bool IsInBounds(int row, int col) const;

    /**
     * @brief Gets a cell at specified coordinates.
     * 
     * @param row Row coordinate
     * @param col Column coordinate
     * @return Shared pointer to the cell, or nullptr if out of bounds
     */
    std::shared_ptr<Cell> GetCell(int row, int col) const;

    /**
     * @brief Gets valid neighboring cells for pathfinding.
     * 
     * @param x Row coordinate
     * @param y Column coordinate
     * @return Vector of valid neighbor coordinates
     */
    std::vector<std::pair<int, int>> GetNeighbors(int x, int y) const;

    /**
     * @brief Calculates movement cost between two cells.
     * 
     * @param row1 Start row
     * @param col1 Start column
     * @param row2 End row
     * @param col2 End column
     * @return Movement cost (1.0 for cardinal, sqrt(2) for diagonal)
     */
    double GetMovementCost(int row1, int col1, int row2, int col2) const;

    /**
     * @brief Calculates heuristic cost between two cells.
     * 
     * @param row1 Start row
     * @param col1 Start column
     * @param row2 End row
     * @param col2 End column
     * @return Heuristic cost (Manhattan or Octile distance)
     */
    double Heuristic(int row1, int col1, int row2, int col2) const;

    /**
     * @brief Prints the map to console.
     */
    void PrintMap() const;

    /**
     * @brief Opens a window to visualize the map.
     */
    void VisualizeMap() const;

    /**
     * @brief Opens a window to visualize the map with a custom title.
     * 
     * @param title Window title
     */
    void VisualizeMapWithTitle(const std::string& title) const;

    /**
     * @brief Gets the map width.
     * 
     * @return Map width in cells
     */
    int GetWidth() const;

    /**
     * @brief Gets the map height.
     * 
     * @return Map height in cells
     */
    int GetHeight() const;

  private:
    std::vector<std::vector<std::shared_ptr<Cell>>> grid;  ///< 2D grid of cells
    std::string map_name;                ///< Name of the map
    int map_height;                      ///< Height of the map
    int map_width;                       ///< Width of the map
    MovementType movement_type;          ///< Movement type (Manhattan or Octile)
};

#endif // MAP_H