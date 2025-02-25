#include "AStar.h"

Node::Node(int _row, int _col, double g, double h, std::shared_ptr<Node> _parent)
  : row(_row), col(_col), g_cost(g), h_cost(h), f_cost(g + h), parent(std::move(_parent)) {}

  const double EPSILON = 1e-6;

bool Node::operator>(const Node& other) const {
    return f_cost > other.f_cost;
}

AStar::AStar(std::shared_ptr<Map> _map) : map(std::move(_map)) {}

std::vector<std::shared_ptr<Cell>> AStar::FindPath(int start_row, int start_col, int goal_row, int goal_col) {
  if (!map->IsInBounds(start_row, start_col) || !map->IsInBounds(goal_row, goal_col)) {
    return {}; // Return empty path if out of bounds
  }

  // Priority queue (min-heap) for open set
  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, std::greater<>> open_set;

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

    if (row == 14 && col == 9) {
      std::cout << "Test\n";
    }

    // If goal reached, reconstruct path
    if (row == goal_row && col == goal_col) {
      return ReconstructPath(current_node);
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

      if (!all_nodes[nrow][ncol] || g_cost < all_nodes[nrow][ncol]->g_cost) {
        auto neighbor_node = std::make_shared<Node>(nrow, ncol, g_cost, h_cost, current_node);
        all_nodes[nrow][ncol] = neighbor_node;
        open_set.push(neighbor_node);
      }
    }
  }

  return {}; // No path found
}

std::vector<std::shared_ptr<Cell>> AStar::ReconstructPath(std::shared_ptr<Node> goal_node) {
  std::vector<std::shared_ptr<Cell>> path;
  auto current = goal_node;

  while (current) {
    path.push_back(map->GetCell(current->row, current->col));
    current = current->parent;
  }

  std::reverse(path.begin(), path.end());
  return path;
}
