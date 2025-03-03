#include "CBS.h"

CBS::CBS(std::shared_ptr<Map> _map, std::vector<std::shared_ptr<Agent>>& _agents) : map(_map), agents(_agents) {}

void CBS::PathFind() {

  int start_time = 0;
  for (size_t agent_id = 0; agent_id < agents.size(); agent_id++) {
    auto& agent = agents[agent_id];
    
    // Create a new AStar object for this agent
    AStar astar(map);

    // Run A* pathfinding for the agent using their start and goal
    std::vector<std::shared_ptr<Cell>> path = astar.FindPath(
      agent->GetAgentData().start_row, agent->GetAgentData().start_col, agent->GetAgentData().dest_row, agent->GetAgentData().dest_col,
      agent_id, start_time
    );

    // Assign character icon (A-Z) based on agent index
    char icon = 'A' + agent_id;  // This will give 'A', 'B', 'C', ..., for agents

    // Update grid cells with the agent's path
    for (auto& cell : path) {
      cell->icon = icon;
      cell->icons.insert(icon);
    }

    // Store the agent's computed path
    agent->SetPath(path);

    //map->VisualizeMap();
  }
}
