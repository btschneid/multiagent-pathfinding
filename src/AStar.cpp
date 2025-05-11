#include "AStar.h"

// Forward declaration for the Constraints
#include "CBS.h"

// Updated Node constructor with time_step
Node::Node(int _row, int _col, double g, double h, int _time_step, std::shared_ptr<Node> _parent)
  : row(_row), col(_col), g_cost(g), h_cost(h), f_cost(g + h), time_step(_time_step), parent(std::move(_parent)) {}

struct NodeComparator {
  bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
    // Compare the f_cost of the underlying Node objects
    return a->f_cost > b->f_cost;  // For min-heap behavior
  }
};

AStar::AStar(std::shared_ptr<Map> _map) : map(std::move(_map)) {}

// Original FindPath method (without constraints) - forward to the new one with empty constraints
std::vector<std::shared_ptr<Cell>> AStar::FindPath(int start_row, int start_col, int goal_row, int goal_col, int agent_id, int start_time) {
    Constraints empty_constraints;
    return FindPath(start_row, start_col, goal_row, goal_col, agent_id, start_time, empty_constraints);
}

// New FindPath method with constraints
std::vector<std::shared_ptr<Cell>> AStar::FindPath(int start_row, int start_col, int goal_row, int goal_col, int agent_id, int start_time, const Constraints& constraints) {
  if (!map->IsInBounds(start_row, start_col) || !map->IsInBounds(goal_row, goal_col)) {
    return {}; // Return empty path if out of bounds
  }

  // Check if start position has a vertex constraint
  if (constraints.hasVertexConstraint(agent_id, start_row, start_col, start_time)) {
    return {}; // Cannot start at a constrained position
  }

  // Priority queue (min-heap) for open set
  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeComparator> open_set;

  // Maps (row, col, time) -> Node to track g_costs
  std::unordered_map<std::string, std::shared_ptr<Node>> all_nodes;

  // Start node with time_step
  auto start_node = std::make_shared<Node>(start_row, start_col, 0.0, 
                                         map->Heuristic(start_row, start_col, goal_row, goal_col), 
                                         start_time, nullptr);
  open_set.push(start_node);
  
  // Use a key that includes time_step to handle waiting at the same location
  std::string start_key = std::to_string(start_row) + "," + std::to_string(start_col) + "," + std::to_string(start_time);
  all_nodes[start_key] = start_node;

  // Set to track visited cells at specific times
  std::unordered_set<std::string> closed_set;

  // Maximum time to avoid infinite loops (e.g., when no valid path exists due to constraints)
  const int MAX_TIME = 1000;

  while (!open_set.empty()) {
    auto current_node = open_set.top();
    open_set.pop();

    int row = current_node->row;
    int col = current_node->col;
    int time = current_node->time_step;
    
    // If we've been searching too long, abort
    if (time > MAX_TIME) {
      return {}; // No path found within time limit
    }

    // If goal reached, reconstruct path
    if (row == goal_row && col == goal_col) {
      return ReconstructPath(current_node, agent_id, start_time);
    }

    std::string key = std::to_string(row) + "," + std::to_string(col) + "," + std::to_string(time);
    if (closed_set.find(key) != closed_set.end()) continue;
    closed_set.insert(key);

    // Add "wait" action - agent stays at the same location
    {
      int next_time = time + 1;
      // Check if waiting violates any constraints
      if (!constraints.hasVertexConstraint(agent_id, row, col, next_time)) {
        double g_cost = current_node->g_cost + 1.0; // Cost of waiting is 1
        
        std::string next_key = std::to_string(row) + "," + std::to_string(col) + "," + std::to_string(next_time);
        if (closed_set.find(next_key) == closed_set.end() && 
            (!all_nodes[next_key] || g_cost < all_nodes[next_key]->g_cost)) {
          
          auto wait_node = std::make_shared<Node>(row, col, g_cost, 
                                               map->Heuristic(row, col, goal_row, goal_col), 
                                               next_time, current_node);
          all_nodes[next_key] = wait_node;
          open_set.push(wait_node);
        }
      }
    }

    // Iterate over neighbors
    for (const auto& [nrow, ncol] : map->GetNeighbors(row, col)) {
      if (map->IsObstacle(nrow, ncol)) {
        continue;
      }
      
      int next_time = time + 1;
      
      // Check if the move violates any constraints (vertex or edge)
      if (ViolatesConstraints(agent_id, row, col, nrow, ncol, time, constraints)) {
        continue;
      }

      double g_cost = current_node->g_cost + map->GetMovementCost(row, col, nrow, ncol);
      double h_cost = map->Heuristic(nrow, ncol, goal_row, goal_col);
      
      std::string next_key = std::to_string(nrow) + "," + std::to_string(ncol) + "," + std::to_string(next_time);
      
      // Check if this node has been visited and whether we can improve the cost
      if (closed_set.find(next_key) == closed_set.end() && 
          (!all_nodes[next_key] || g_cost < all_nodes[next_key]->g_cost)) {
        
        // Create a new node with the updated costs and parent
        auto neighbor_node = std::make_shared<Node>(nrow, ncol, g_cost, h_cost, next_time, current_node);
        
        // Store the new node
        all_nodes[next_key] = neighbor_node;
        
        // Push the neighbor node to the open set
        open_set.push(neighbor_node);
      }
    }
  }

  return {}; // No path found
}

// Helper function to check if a move violates constraints
bool AStar::ViolatesConstraints(int agent_id, int row1, int col1, int row2, int col2, int time, const Constraints& constraints) {
  // Check vertex constraint on the next position
  if (constraints.hasVertexConstraint(agent_id, row2, col2, time + 1)) {
    return true;
  }
  
  // Check edge constraint for the move
  if (constraints.hasEdgeConstraint(agent_id, row1, col1, row2, col2, time)) {
    return true;
  }
  
  return false;
}

std::vector<std::shared_ptr<Cell>> AStar::ReconstructPath(std::shared_ptr<Node> goal_node, int agent_id, int start_time) {
  std::vector<std::shared_ptr<Cell>> path;
  auto current = goal_node;
  
  // Determine the path length (time steps)
  int max_time = current->time_step;
  
  // Initialize the path with the right size
  path.resize(max_time - start_time + 1);
  
  // Fill the path from goal to start
  while (current) {
    int time_index = current->time_step - start_time;
    if (time_index >= 0 && time_index < path.size()) {
      std::shared_ptr<Cell> cell = map->GetCell(current->row, current->col);
      
      // Update the occupancy map with the agent ID at the current time
      cell->occupancy_map[current->time_step].push_back(agent_id);
      
      // Store the cell at the right position in the path
      path[time_index] = cell;
    }
    
    // Move to the parent node to continue reconstructing the path
    current = current->parent;
  }
  
  return path;
}