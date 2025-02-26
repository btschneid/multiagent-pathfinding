#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <unordered_set>
#include <cmath>
#include "Agent.h"
#include "Map.h"
#include <algorithm>

struct Node {
  std::shared_ptr<Cell> cell;  // The cell itself
  int row, col;                    // Coordinates of the cell
  double g_cost;               // Cost from start node
  double h_cost;               // Heuristic cost to goal
  double f_cost;               // Total cost (g + h)
  std::shared_ptr<Node> parent; // Pointer to parent node for path reconstruction

  Node(int _row, int _col, double g, double h, std::shared_ptr<Node> _parent);

  // Comparison operator for priority queue
  bool operator>(const Node& other) const;
};

class AStar {
  public:
    AStar(std::shared_ptr<Map> _map);

    // Finds the shortest path from (start_x, start_y) to (goal_x, goal_y)
    std::vector<std::shared_ptr<Cell>> FindPath(int start_row, int start_col, int goal_row, int goal_col);

  private:
    std::shared_ptr<Map> map;

    // Reconstructs the path from the goal node
    std::vector<std::shared_ptr<Cell>> ReconstructPath(std::shared_ptr<Node> goal_node);

};

#endif // CBS_H
