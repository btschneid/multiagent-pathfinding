#include "AStar.h"

Node::Node(int _row, int _col, double g, double h, std::shared_ptr<Node> _parent)
  : row(_row), col(_col), g_cost(g), h_cost(h), f_cost(g + h), parent(std::move(_parent)) {}

struct NodeComparator {
  bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
    // Compare the f_cost of the underlying Node objects
    return a->f_cost > b->f_cost;  // For min-heap behavior
  }
};


AStar::AStar(std::shared_ptr<Map> _map) : map(std::move(_map)) {}

std::vector<std::shared_ptr<Cell>> AStar::FindPath(int start_row, int start_col, int goal_row, int goal_col, int agent_id, int start_time) {
  if (!map->IsInBounds(start_row, start_col) || !map->IsInBounds(goal_row, goal_col)) {
    return {}; // Return empty path if out of bounds
  }

  // Priority queue (min-heap) for open set
  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> open_set;

  // Maps (row, col) -> Node to track g_costs
  std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Node>>> all_nodes;

  // Start node
  auto start_node = std::make_shared<Node>(start_row, start_col, 0.0, map->Heuristic(start_row, start_col, goal_row, goal_col), nullptr);
  open_set.push(start_node);
  all_nodes[start_row][start_col] = start_node;

  // Set to track visited cells
  std::unordered_set<std::string> closed_set;

  while (!open_set.empty()) {
    auto current_node = open_set.top();
    open_set.pop();

    int row = current_node->row;
    int col = current_node->col;

    // If goal reached, reconstruct path
    if (row == goal_row && col == goal_col) {
      return ReconstructPath(current_node, agent_id, start_time);
    }

    std::string key = std::to_string(row) + "," + std::to_string(col);
    if (closed_set.find(key) != closed_set.end()) continue;
    closed_set.insert(key);

    // Iterate over neighbors
    for (const auto& [nrow, ncol] : map->GetNeighbors(row, col)) {
      if (map->IsObstacle(nrow, ncol) || closed_set.find(std::to_string(nrow) + "," + std::to_string(ncol)) != closed_set.end()) {
        continue;
      }

      double g_cost = current_node->g_cost + map->GetMovementCost(row, col, nrow, ncol);
      double h_cost = map->Heuristic(nrow, ncol, goal_row, goal_col);

      // Check if this node has been visited and whether we can improve the cost
      if (!all_nodes[nrow][ncol] || g_cost < all_nodes[nrow][ncol]->g_cost) {
        // Create a new node with the updated costs and parent
        auto neighbor_node = std::make_shared<Node>(nrow, ncol, g_cost, h_cost, current_node);
        
        // If the node was already created, update the parent and costs
        if (all_nodes[nrow][ncol]) {
          all_nodes[nrow][ncol]->g_cost = g_cost;
          all_nodes[nrow][ncol]->f_cost = g_cost + h_cost;  // Recalculate the f_cost
          all_nodes[nrow][ncol]->parent = current_node;  // Update the parent to current node
        }
        
        // Store the new or updated node
        all_nodes[nrow][ncol] = neighbor_node;
        
        // Push the neighbor node to the open set
        open_set.push(neighbor_node);
      }
    }
  }

  return {}; // No path found
}

std::vector<std::shared_ptr<Cell>> AStar::ReconstructPath(std::shared_ptr<Node> goal_node, int agent_id, int start_time) {
  std::vector<std::shared_ptr<Cell>> path;
  auto current = goal_node;
  int time = start_time;

  while (current) {
    std::shared_ptr<Cell> cell = map->GetCell(current->row, current->col);
    
    // Update the occupancy map with the agent ID at the current time
    cell->occupancy_map[time].push_back(agent_id);

    // Add the current cell to the path
    path.push_back(cell);

    // Increment time (or use any logic to advance time based on your simulation)
    time++;

    // Move to the parent node to continue reconstructing the path
    current = current->parent;
  }

  // Reverse the path to get it from start to goal
  std::reverse(path.begin(), path.end());

  return path;
}

