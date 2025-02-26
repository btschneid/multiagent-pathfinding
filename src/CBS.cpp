#include "CBS.h"

CBS::CBS(std::shared_ptr<Map> _map, std::vector<std::shared_ptr<Agent>>& _agents) : map(_map), agents(_agents) {}

void CBS::PathFind() {
  for (size_t i = 0; i < agents.size(); i++) {
      auto& agent = agents[i];
      
      // Create a new AStar object for this agent
      AStar astar(map);

      // Run A* pathfinding for the agent using their start and goal
      std::vector<std::shared_ptr<Cell>> path = astar.FindPath(
          agent->GetAgentData().start_row, agent->GetAgentData().start_col, agent->GetAgentData().dest_row, agent->GetAgentData().dest_col
      );

      // Assign character icon (A-Z) based on agent index
      char icon = 'A' + i;  // This will give 'A', 'B', 'C', ..., for agents

      // Update grid cells with the agent's path
      for (auto& cell : path) {
          cell->icon = icon;
      }

      // Store the agent's computed path
      agent->SetPath(path);
  }
}
