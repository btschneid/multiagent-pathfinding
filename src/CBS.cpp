#include "CBS.h"

bool VertexConstraint::operator==(const VertexConstraint& other) const {
  return agent_id == other.agent_id && row == other.row && 
         col == other.col && time == other.time;
}

bool EdgeConstraint::operator==(const EdgeConstraint& other) const {
  return agent_id == other.agent_id && row1 == other.row1 && 
         col1 == other.col1 && row2 == other.row2 && 
         col2 == other.col2 && time == other.time;
}

bool Constraints::hasVertexConstraint(int agent_id, int row, int col, int time) const {
  return vertex_constraints.count(VertexConstraint{agent_id, row, col, time}) > 0;
}

bool Constraints::hasEdgeConstraint(int agent_id, int row1, int col1, int row2, int col2, int time) const {
  return edge_constraints.count(EdgeConstraint{agent_id, row1, col1, row2, col2, time}) > 0;
}

bool CBSNode::operator>(const CBSNode& other) const {
  return cost > other.cost;
}

CBS::CBS(std::shared_ptr<Map> _map, std::vector<std::shared_ptr<Agent>>& _agents) : map(_map), agents(_agents) {}

void CBS::PathFind() {
  // Create root node
  CBSNode root;
  root.constraints = Constraints();  // Empty constraints
  root.solution.resize(agents.size());
  
  // Find initial paths for all agents
  AStar astar(map);
  bool all_paths_found = true;
  
  for (size_t i = 0; i < agents.size(); i++) {
    auto& agent = agents[i];
    auto path = astar.FindPath(
      agent->GetAgentData().start_row, 
      agent->GetAgentData().start_col, 
      agent->GetAgentData().dest_row, 
      agent->GetAgentData().dest_col, 
      i, 0, root.constraints
    );
    
    if (path.empty()) {
      all_paths_found = false;
      break;
    }
    
    root.solution[i] = path;
  }
  
  if (!all_paths_found) {
    std::cerr << "No solution exists for at least one agent" << std::endl;
    return;
  }
  
  // Calculate the cost of the root solution
  root.cost = CalculateSolutionCost(root.solution);
  
  // Find conflicts in the root solution
  root.conflict = FindConflict(root.solution);
  
  // Priority queue for CBS nodes
  std::priority_queue<CBSNode, std::vector<CBSNode>, std::greater<CBSNode>> open_list;
  open_list.push(root);
  
  // Main CBS loop
  int max_iterations = 100; // Set a reasonable maximum number of iterations
  int iteration_count = 0;
  
  while (!open_list.empty()) {
    // Check if we've exceeded the maximum iterations
    if (iteration_count >= max_iterations) {
      std::cerr << "No solution found within " << max_iterations << " iterations" << std::endl;
      return;
    }
    
    // Get the node with the lowest cost
    CBSNode current = open_list.top();
    open_list.pop();
    
    // If there's no conflict, we found a solution
    if (!current.conflict) {
      // Update the map with the final solution
      UpdateMapWithSolution(current.solution);
      
      // Update each agent's path
      for (size_t i = 0; i < agents.size(); i++) {
        agents[i]->SetPath(current.solution[i]);
      }
      
      std::cout << "Solution found with cost: " << current.cost << std::endl;
      return;
    }
      
    // Split the node into two branches
    for (int branch = 0; branch < 2; branch++) {
      CBSNode new_node = current;
      int constrained_agent = (branch == 0) ? current.conflict->agent1_id : current.conflict->agent2_id;
      
      // Add new constraint based on conflict type
      if (current.conflict->type == Conflict::VERTEX) {
          // Vertex conflict: Agent cannot be at (row, col) at time
          VertexConstraint vc{
            constrained_agent,
            current.conflict->row,
            current.conflict->col,
            current.conflict->time
          };
          new_node.constraints.vertex_constraints.insert(vc);
      } else {
        // Edge conflict: Agent cannot move from (row1, col1) to (row2, col2) at time
        EdgeConstraint ec{
          constrained_agent,
          current.conflict->row1,
          current.conflict->col1,
          current.conflict->row2,
          current.conflict->col2,
          current.conflict->time
        };
        new_node.constraints.edge_constraints.insert(ec);
      }
      
      // Replan for the constrained agent
      auto new_path = astar.FindPath(
        agents[constrained_agent]->GetAgentData().start_row,
        agents[constrained_agent]->GetAgentData().start_col,
        agents[constrained_agent]->GetAgentData().dest_row,
        agents[constrained_agent]->GetAgentData().dest_col,
        constrained_agent, 0, new_node.constraints
      );
      
      // If path was found, update solution and continue
      if (!new_path.empty()) {
        new_node.solution[constrained_agent] = new_path;
        new_node.cost = CalculateSolutionCost(new_node.solution);
        new_node.conflict = FindConflict(new_node.solution);
        open_list.push(new_node);
      }
    }
    
    iteration_count++;
  }
  
  std::cerr << "No solution found for all agents after exploring all possible paths" << std::endl;
}

std::shared_ptr<Conflict> CBS::FindConflict(const std::vector<std::vector<std::shared_ptr<Cell>>>& solution) {
  // Check for conflicts between each pair of agents
  for (size_t i = 0; i < solution.size(); i++) {
    for (size_t j = i + 1; j < solution.size(); j++) {
      int conflict_time = 0;
      std::shared_ptr<Conflict> conflict = std::make_shared<Conflict>();
      
      if (HasConflict(solution[i], solution[j], conflict_time, conflict)) {
        conflict->agent1_id = i;
        conflict->agent2_id = j;
        conflict->time = conflict_time;
        return conflict;
      }
    }
  }
  
  return nullptr; // No conflicts found
}

bool CBS::HasConflict(const std::vector<std::shared_ptr<Cell>>& path1, 
                       const std::vector<std::shared_ptr<Cell>>& path2, 
                       int& conflict_time,
                       std::shared_ptr<Conflict>& conflict) {
  // Get the maximum path length
  size_t max_length = std::max(path1.size(), path2.size());
  
  // Check for vertex conflicts (agents at the same position at the same time)
  for (size_t t = 0; t < max_length; t++) {
    // If path ended, assume agent stays at its last position
    size_t idx1 = std::min(t, path1.size() - 1);
    size_t idx2 = std::min(t, path2.size() - 1);
    
    // Check vertex conflict
    if (path1[idx1]->row == path2[idx2]->row && path1[idx1]->col == path2[idx2]->col) {
      conflict_time = t;
      conflict->type = Conflict::VERTEX;
      conflict->row = path1[idx1]->row;
      conflict->col = path1[idx1]->col;
      return true;
    }
    
    // Check edge conflict (agents swap positions)
    if (t > 0 && 
      path1[idx1]->row == path2[idx2 - 1]->row && path1[idx1]->col == path2[idx2 - 1]->col &&
      path1[idx1 - 1]->row == path2[idx2]->row && path1[idx1 - 1]->col == path2[idx2]->col) {
      
      conflict_time = t - 1;
      conflict->type = Conflict::EDGE;
      conflict->row1 = path1[idx1 - 1]->row;
      conflict->col1 = path1[idx1 - 1]->col;
      conflict->row2 = path1[idx1]->row;
      conflict->col2 = path1[idx1]->col;
      return true;
    }
  }
  
  return false; // No conflict found
}

std::vector<std::shared_ptr<Cell>> CBS::FindPath(int agent_id, const Constraints& constraints) {
  AStar astar(map);
  auto& agent = agents[agent_id];
  
  return astar.FindPath(
    agent->GetAgentData().start_row,
    agent->GetAgentData().start_col,
    agent->GetAgentData().dest_row,
    agent->GetAgentData().dest_col,
    agent_id, 0, constraints
  );
}

int CBS::CalculateSolutionCost(const std::vector<std::vector<std::shared_ptr<Cell>>>& solution) {
  int total_cost = 0;
  
  for (const auto& path : solution) {
    // Path cost is its length
    total_cost += path.size();
  }
  
  return total_cost;
}

void CBS::UpdateMapWithSolution(const std::vector<std::vector<std::shared_ptr<Cell>>>& solution) {
  // Reset the map first (clear all agent data)
  for (int r = 0; r < map->GetHeight(); r++) {
    for (int c = 0; c < map->GetWidth(); c++) {
      auto cell = map->GetCell(r, c);
      if (cell && !cell->IsObstacle()) {
        cell->occupancy_map.clear();
        cell->icon = '.';
        cell->icons.clear();
      }
    }
}

// Add each agent's path to the map
for (size_t agent_id = 0; agent_id < solution.size(); agent_id++) {
    char icon = 'A' + agent_id;  // This will give 'A', 'B', 'C', ..., for agents
    
    for (size_t t = 0; t < solution[agent_id].size(); t++) {
      auto& cell = solution[agent_id][t];
      cell->icon = icon;
      cell->icons.insert(icon);
      cell->occupancy_map[t].push_back(agent_id);
    }
  }
}