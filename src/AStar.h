/**
 * @file AStar.h
 * @brief Defines the A* pathfinding algorithm and related structures.
 * 
 * This file implements the A* algorithm for finding optimal paths in a grid,
 * including time-dependent constraints for multi-agent pathfinding.
 */

#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <algorithm>
#include "Map.h"

// Forward declaration for Constraints
struct Constraints;
struct VertexConstraint;
struct EdgeConstraint;

/**
 * @brief Represents a node in the A* search tree.
 * 
 * Each node contains position information, path costs, and a pointer
 * to its parent node for path reconstruction.
 */
class Node {
public:
    int row, col;           ///< Grid coordinates
    double g_cost;          ///< Cost from start to this node
    double h_cost;          ///< Heuristic cost to goal
    double f_cost;          ///< Total cost (g_cost + h_cost)
    int time_step;          ///< Time step for time-dependent planning
    std::shared_ptr<Node> parent;  ///< Pointer to parent node

    /**
     * @brief Constructs a new Node object.
     * 
     * @param _row Row coordinate
     * @param _col Column coordinate
     * @param g Cost from start
     * @param h Heuristic cost to goal
     * @param _time_step Time step
     * @param _parent Pointer to parent node
     */
    Node(int _row, int _col, double g, double h, int _time_step, std::shared_ptr<Node> _parent = nullptr);
};

/**
 * @brief Implements the A* pathfinding algorithm.
 * 
 * The AStar class provides functionality for finding optimal paths
 * in a grid, with support for time-dependent constraints.
 */
class AStar {
public:
    /**
     * @brief Constructs a new A* solver.
     * 
     * @param _map Shared pointer to the map
     */
    AStar(std::shared_ptr<Map> _map);
    
    /**
     * @brief Finds a path without constraints.
     * 
     * @param start_row Starting row
     * @param start_col Starting column
     * @param goal_row Goal row
     * @param goal_col Goal column
     * @param agent_id ID of the agent
     * @param start_time Starting time step
     * @return Vector of cells representing the path
     */
    std::vector<std::shared_ptr<Cell>> FindPath(int start_row, int start_col, 
                                              int goal_row, int goal_col, 
                                              int agent_id, int start_time);
    
    /**
     * @brief Finds a path with constraints.
     * 
     * @param start_row Starting row
     * @param start_col Starting column
     * @param goal_row Goal row
     * @param goal_col Goal column
     * @param agent_id ID of the agent
     * @param start_time Starting time step
     * @param constraints Set of constraints to satisfy
     * @return Vector of cells representing the path
     */
    std::vector<std::shared_ptr<Cell>> FindPath(int start_row, int start_col, 
                                              int goal_row, int goal_col, 
                                              int agent_id, int start_time, 
                                              const Constraints& constraints);

private:
    std::shared_ptr<Map> map;  ///< Shared pointer to the map
    
    /**
     * @brief Reconstructs the path from a goal node.
     * 
     * @param goal_node Pointer to the goal node
     * @param agent_id ID of the agent
     * @param start_time Starting time step
     * @return Vector of cells representing the path
     */
    std::vector<std::shared_ptr<Cell>> ReconstructPath(std::shared_ptr<Node> goal_node, 
                                                     int agent_id, int start_time);
    
    /**
     * @brief Checks if a move violates any constraints.
     * 
     * @param agent_id ID of the agent
     * @param row1 Starting row
     * @param col1 Starting column
     * @param row2 Ending row
     * @param col2 Ending column
     * @param time Time step
     * @param constraints Set of constraints to check
     * @return true if the move violates any constraints
     */
    bool ViolatesConstraints(int agent_id, int row1, int col1, 
                           int row2, int col2, int time, 
                           const Constraints& constraints);
};

#endif // ASTAR_H